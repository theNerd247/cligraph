/*
 * plugins.c is part of cligraph.
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
 * plugins.c 
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

typedef struct dlmap_node_st
{
	void* handle;
	char* libname;
	pthread_t* thread;
} DLNode;

static LList* dlmap; 

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

DLNode* gettuinode()
{
	DLNode* nd = NULL;
	size_t filter(void* data)
	{
		DLNode* node = (DLNode*)data;
		if(!strcmp(node->libname,"tui"))
		{
			nd = node;
			return 1;
		}
		return 0;
	}
	
	llapply(dlmap,&filter);
	return nd;

}

//helper function for main and startplugins
/* fetches the thread attached to the tui plugin */
pthread_t* gettuithread()
{
	return gettuinode()->thread;
}

void startplugin(DLNode* data)
{
	char* lbname = (char*)(data->libname);
	char funcname[strlen(lbname)+5]; 
	void *(*strtfunc)(void *); 
	int error_code = 0;
	data->thread = (pthread_t*)malloc(sizeof(pthread_t));

	//create the function name
	strcpy(funcname,"start");
	strcat(funcname,lbname);
	
	//create the thread
	log_attempt("Starting plugin %s",lbname);
	error_run(strtfunc = getfuncref(lbname,funcname),log_failure("Could not load start func"));
	error_run(!(error_code = pthread_create(data->thread, NULL, strtfunc, NULL)), log_failure("Error code: %i",error_code));
	log_success();

	error:
	return;
}

void startplugins()
{
	//manually start the tui plugin since it needs to be first
	//TODO: fix this hack
	log_attempt("Starting plugin tui");
	void* (*strtfunc)(void*);

	DLNode* tuinode = gettuinode();	
	tuinode->thread = (pthread_t*)malloc(sizeof(pthread_t));

	char error_code = 0;
	error_run(strtfunc = getfuncref("tui","starttui"),log_failure("Could not load start func"));
	error_run(!(error_code = pthread_create(tuinode->thread, NULL, strtfunc, NULL)), log_failure("Error code: %i",error_code));
	log_success();

	size_t strtp(void* data)
	{
		if(!data) return 0;
		char* lbname = (char*)((DLNode*)data)->libname;
		if(!strcmp(lbname,"tui")) return 0;//ignore the tui 
		startplugin((DLNode*)data);
		return 0;
	}

	check(llapply(dlmap,&strtp),"traversion function failed");
	error:
		return;
}
