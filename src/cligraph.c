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
#include <pthread.h>
#include <llist.h>

#include "cligraph.h"
#include "tui.h"

//--SEMI-INCLUDES------------------------------
/*
 * In order for the functions in plugins.c to work we need to
 * manually define the functions for main to work below
 */
LList* getliblist();
void startplugins();
void unloadplugins();
//--END SEMI-INCLUDES---------------------------

static pthread_mutex_t wait_mutex;
static pthread_cond_t wait_cond;
static unsigned char wait_sig;

//--LOGGING------------------------------
//#define NLOG //uncomment to turn off logging 
#include "dbg.h"

#define __LOG_FILE_PATH "log.txt"
#define __WRITE_MODE "w"
//--END LOGGING---------------------------

int main(int argc, char const *argv[])
{		
	//init variables 
	int error_code = 0;
	pthread_t tui_thread;

	//move all stdout prints to a log file
	freopen(__LOG_FILE_PATH,__WRITE_MODE,stderr);

	//TODO<sanity checks and option parsing goes here>

	//start UI
	check(!(error_code = pthread_create(&tui_thread,NULL,&starttui,NULL)), "could not start tui thread: %i",error_code);

	//load plugins
	if(!getliblist()) fprintf(stderr,"no plugins found\n");
	else
		startplugins();

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
