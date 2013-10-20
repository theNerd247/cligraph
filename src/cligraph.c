/*
 * cligraph.c is part of cligraph.
 * 
 * cligraph is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * cligraph is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with cligraph.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * cligraph.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: main source file for cligraph
 */

#include <dirent.h> 
#include <dlfcn.h> 
#include <errno.h> 
#include <llist.h>
#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 

#include "cligraph.h"

//#define NLOG //uncomment to turn off logging 
#include "dbg.h"

//config stuff 
#define PLUGIN_PATH "plugin"
#define DL_LOAD_FLAG (RTLD_LAZY || RTLD_GLOBAL)

typedef struct dlmap_node_st
{
	void* handle;
	char* libname;
} DLNode;

LList* dlmap; 

/*
 * helper function for getfuncref()
 * fetches the handle given by the libname
 * the current algo may be changed to something more efficient
 *  */
void* gethandle(char* libname)
{
	//assume libname checks have already been performed by getfuncref
	void* handle = NULL;
	size_t chck_name(void* data)
	{
		DLNode* dlnode = (DLNode*)data;
		if(strcmp(dlnode->libname, libname) == 0)
		{
			handle = dlnode->handle;
			return 1;
		}
		return 0;
	}

	if(!(llapply(dlmap, &chck_name))) return NULL;

	return handle;
}

void* getfuncref(char* libname, char* funcname)
{
	log_attempt("Loading func \"%s\" in lib \"%s\"",funcname,libname);
	void* handle;
	void* func;
	//sanity checks
	error_run((libname && funcname),log_failure("libname or funcname not valid")) 
	error_run((strcmp(libname,"") != 0 && strcmp(funcname, "") != 0), log_failure("libname or funcname not valid")); 

	//find handle relating to libname
	error_run((handle = gethandle(libname)), log_failure("could not fetch handle"));

	//get function refrence
	error_run((func = dlsym(handle, funcname)), log_failure("%s",dlerror()));

	log_success();
	return func;

	error:
		return NULL;
}

/* helper function for getliblist
 * loads the library given the directory name
 * if the shared-object file cannot be found then
 * the code is attempted to be compiled.
 */
DLNode* compileplin(const char* libname)
{
	//init vars
	void* handle;
	DLNode* dlnode;

	//assume dirname is a valid string because we are, of course, a helper
	//function
	//set up path of .so file
	char dirname[strlen(libname)*2+strlen(PLUGIN_PATH)+9];
	strcpy(dirname,PLUGIN_PATH);
	strcat(dirname,"/");
	strcat(dirname,libname);
	strcat(dirname,"/lib");
	strcat(dirname,libname);
	strcat(dirname,".so");

	//load library based on the dirname
	error_run(handle = dlopen(dirname,DL_LOAD_FLAG),(void)"");

	//if all goes well create a new DLNode to save the info 
	//for later calling
	error_run(dlnode = (DLNode*)malloc(sizeof(DLNode)), errno = ENOMEM);
	dlnode->handle = handle;

	error_run(dlnode->libname = (char*)malloc(sizeof(char)*strlen(libname)+1), errno = ENOMEM);
	strcpy(dlnode->libname,libname);

	return dlnode;

	error: 
		return NULL;
}

//auto detects, compiles, and loads the plugins
LList* getliblist()
{
	//init vars
	DLNode* dlnode; //holds the newly loaded handle
	char errstat = 1; //do we return from this function if error occurs?

	//init dlmap
	dlmap = llnew();
	if(!dlmap) return NULL;

	//open plugin-directory
	DIR* dir;
	if(!(dir = opendir(PLUGIN_PATH))) return NULL;		
	
	struct dirent* fl = readdir(dir);
	while(fl)
	{
		//ignore current and parent directories
		if(strcmp(fl->d_name,".") == 0 || strcmp(fl->d_name,"..") == 0) 
		{
			fl = readdir(dir);
			continue; 
		}

		//search plugindir for directories
		if(fl->d_type == DT_DIR) 
		{
			errstat = 0;

			//trim the d_dname to something proper
			char dirname[strlen(fl->d_name)+1];
			strcpy(dirname,fl->d_name);

			log_attempt("Plugin \"%s\" found attempting to load it",dirname);

			//create new node
			error_run(dlnode = compileplin(dirname), log_failure("%s",dlerror())); //compileplin will give detailed error message if needed
			error_run(llappend(dlmap,dlnode), log_failure("could not cache"));
			log_success();
		}
		reset:
			errstat = 1;

		//go to next entry in dirlist
		fl = readdir(dir);
	}

	closedir(dir);
	return dlmap->length > 0 ? dlmap : NULL;

	error:
		if(errstat)
			return NULL;
		else 
		{
			free(dlnode);
			goto reset;
		}
}

//free all the memory of these plugins by unloading the 
//shared libraries
void unloadplugins()
{
	log_attempt("Unloading all plugins");
	size_t unloaddl(void* data)
	{
		DLNode* node = (DLNode*)data;
		dlclose(node->handle);
		free(node->libname);
		return 0;
	}	

	error_run(llapply(dlmap, &unloaddl),log_failure("llapply failed see libllist"));

	log_success();
	error: 
		free(dlmap);
}

int main(int argc, char const *argv[])
{		
	//init variables 
	int error_code = 0;
	void* (*starttui)(void*);
	pthread_t tui_thread;

	//sanity checks and option parsing
	//load plugins
	if(!getliblist()) return EXIT_FAILURE;	

	//load the functions to use
	error_run(starttui = getfuncref("tui","starttui"),(void)(0));

	//start running TUI thread
	log_attempt("Starting cligraph");
	error_run(!(error_code = pthread_create(&tui_thread, NULL, starttui, NULL)), log_failure("Error code: %i",error_code));
	log_success();

	//wait for user to exit tui
	check(!(error_code = pthread_join(tui_thread, NULL)),"pthead_join returned: %i",error_code);

	//free all memory
	log_info("Stopping cligraph...");

	unloadplugins();
	exit(EXIT_SUCCESS);

	error:
		unloadplugins();
		exit(EXIT_FAILURE);
} 
