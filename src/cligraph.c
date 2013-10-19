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

#include <stdio.h> 
#include <stdlib.h> 
#include <dlfcn.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <string.h> 
#include <llist.h>
#include <errno.h> 

#include "cligraph.h"

//catch run-time errors
#define __ERRMSG(message) fprintf(stderr,"In file %s:%s: %s -- %s", __FILE__, __LINE__, (strerror(errno)), message)
#define CATCH_RUN(message) if(errno) (__ERRMSG(message), exit(EXIT_FAILURE));
#define CATCH_ERR(message) if(errno) (__ERRMSG(message));
#define CATCH(a) if(!(a)) (__ERRMSG(""), exit(EXIT_FAILURE));
#define CATCH_ASSERT(a,message) if(!(a)) (__ERRMSG(message)); //this gives a segfault....fix it.

#ifndef NDEBUG
#define DEBUG printf
#endif

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
	void* handle;
	//sanity checks
	if(!libname || !funcname) return NULL;
	if(strpbrk(libname," ") || strcmp(libname,"") == 0) return NULL; 
	if(strpbrk(funcname," ") || strcmp(funcname,"") == 0) return NULL;

	//find handle relating to libname
	if(!(handle = gethandle(libname))) return NULL;

	//get function refrence
	//return function ref
	return dlsym(handle, funcname);
}

/* helper function for getliblist
 * loads the library given the directory name
 * if the shared-object file cannot be found then
 * the code is attempted to be compiled.
 */
DLNode* compileplin(char* dirname)
{
	//assume dirname is a valid string because we are, of course, a helper
	//function
	//set up path of .so file
	char libname[strlen(dirname)*2+strlen(PLUGIN_PATH)+9];
	strcpy(libname,PLUGIN_PATH);
	strcat(libname,"/");
	strcat(libname,dirname);
	strcat(libname,"/lib");
	strcat(libname,dirname);
	strcat(libname,".so");
//	CATCH_ERR("")

	//load library based on the dirname
	void* handle = dlopen(libname,DL_LOAD_FLAG);
//	CATCH_ERR("");

	//if the library is not found do stuff
	if(!(handle)) return NULL; //for now we'll just return NULL

	//if all goes well create a new DLNode to save the info 
	//for later calling
	DLNode* dlnode = (DLNode*)malloc(sizeof(DLNode));
	dlnode->handle = handle;
	dlnode->libname = dirname;

	return dlnode;
}

//free all the memory of these plugins by unloading the 
//shared libraries
void unloadplugins()
{
	size_t unloaddl(void* data)
	{
		DLNode* node = (DLNode*)data;
		dlclose(node->handle);
		return 0;
	}	

	llapply(dlmap, &unloaddl);
}

//auto detects, compiles, and loads the plugins
LList* getliblist()
{
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
			//trim the d_dname to something proper
			char* dirname = (char*)malloc(sizeof(char)*strlen(fl->d_name)+1);
			strcpy(dirname,fl->d_name);

			DEBUG("Attemping to load plugin \"%s\"...",dirname);
			//create new node
			DLNode* dlnode = compileplin(dirname);
			if(dlnode) 
			{
				DEBUG("success\n");
				llappend(dlmap,dlnode);
			}
			else
			{
				DEBUG("failed\n");
				free(dirname);
			}
		}

		//go to next entry in dirlist
		fl = readdir(dir);
	}

	closedir(dir);
	return dlmap->length > 0 ? dlmap : NULL;
}

int main(int argc, char const *argv[])
{		
	//init variables 
	//sanity checks and option parsing
	//load plugins
	if(!getliblist()) return EXIT_FAILURE;	

	//start running TUI thread
	int (*starttui)() = getfuncref("tui","starttui");
	starttui();

	//free all memory
	void (*stoptui)() = getfuncref("tui","stoptui");
	stoptui();

	unloadplugins();
	lldestroy(dlmap);
	return 0;
} 
