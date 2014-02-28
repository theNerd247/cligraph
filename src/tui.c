/*
 * tui.c is part of cligraph.
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
 * tui.c 
 *
 * AUTHOR: Noah Harvey
 * 
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: definition file for prototypes in window.h
 */

/** TODO
 * - Restructure the keyboard controller to handle keyevents more efficiently
 * - Make the keyboard controller memory safe
 * - Make the keyboard controller thread safe
 */

#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h> 

#define NDEBUG
#include "dbg.h"
#include "cligraph.h"
#include "tui.h"
#include "keyboard.h"
#include "winmgr.h"

//--THREADS------------------------------
static pthread_t kbthread;
static pthread_t winthread;

static pthread_mutex_t wait_mutex;
static pthread_cond_t wait_cond;
static unsigned char wait_sig;
//--END THREADS---------------------------

/* status of the tui manger */

//the input buffer for the command bar
static char cmdbuff[10000];
static int cmdbuff_index;//used by printable_event to place characters pressed inside the cmdbuff

/* the cmd event for the command bar (performed when enter key is pressed inside
 * the cmdbar */
int cmdbar_cmdevent(WINDOW* cmdbar)
{
	int (*graph)(char*) = (int (*)(char*))getfuncref("graph","graphfunc");
	graph(cmdbuff);

	//clear the cmdbar and the cmdbuffer
	size_t i;
	for (i = 0; i <= cmdbuff_index; i++)
		cmdbuff[i] = '\0';

	cmdbuff_index = 0;

	//clear the command bar
	wmove(cmdbar,1,0);
	wclrtoeol(cmdbar);
	wnoutrefresh(cmdbar);

	return 0;
}

int printdispwin(char* buff)
{
	mvwprintw(getdispwin(),0,0,"%s",buff);
	wnoutrefresh(getdispwin());
	return 0;
}

//--starttui()------------------------------

/*the default action to take place when a printable key is pressed */
int printable_event(int curr_key)
{
	//first write to the input buffer...
	cmdbuff[cmdbuff_index++] = curr_key;

	//...and then print to the output window if printable
	WINDOW* curr_win = getcmdbar();
	waddch(curr_win,curr_key); 
	wnoutrefresh(curr_win);

	return 0;
}

//helper function for starttui
/* sets up the default key events */
int __add_default_keys()
{
	//set all the printable keys to print to the command bar	
	size_t i;
	for (i = PRINTKEY_RANGE_MIN; i <= PRINTKEY_RANGE_MAX; i++)
		check_error(!addkeyevent(i,&printable_event));

	//--other default events------------------------------
	//ESC closes the tui
	check_error(!addkeyevent(ESC_KEY, (event_func_type)stoptui)); 

	//sendcmd is defined in keyboard.c
	check_error(!addkeyevent(ENTER_KEY, sendcmd));
	check_error(!addcmdevent(getcmdbar(),cmdbar_cmdevent));

	//--END other default events---------------------------
	return 0;

	error:
		return 1;
}

void tui_ready()
{
	debug("tui_ready called");
	lock(wait_mutex);
	wait_sig = 1;	
	unlock(wait_mutex);
	pthread_cond_signal(&wait_cond);
}

///helper function for starttui
/** waits for the ready signal and then returns */
void tui__wait()
{
	lock(wait_mutex);
	while(wait_sig == 0) pthread_cond_wait(&wait_cond, &wait_mutex);
	//reset the signal so this function will work next time called
	wait_sig = 0;
	unlock(wait_mutex);
	return;
}

/* sets up and starts running the tui */
void* starttui(void* null)
{
	//vars
	int error_code = 0;

	//init print buffer
	cmdbuff[0] = '\0';
	cmdbuff_index = 0;

	//init thread mutexes and conditions
	pthread_mutex_init(&wait_mutex, NULL);
	pthread_cond_init(&wait_cond, NULL);

	//initial ncurses calls 
	initscr();

	//start the window manager
	error_code = pthread_create(&winthread,NULL,startwinmgr,NULL);
	check(!error_code, "could not start window manager....aborting");
	//wait till the winmgr says it's ok to move on
	tui__wait();

	//start the keyboard controller
	WINDOW* cmdbar = getcmdbar();
	error_code = pthread_create(&kbthread,NULL,startkeyctlr,(void*)cmdbar);
	check(!error_code, "could not start keyboard controller....aborting");
	//wait till the kbdmgr says it's ok to move on
	tui__wait();
	
	//setup the default key and cmd events
	check(!__add_default_keys(), "failed to add default keys....aborting");
	
	//move the cursor to the CMDBAR
	wmove(getcmdbar(),1,0);

	//let parent thread know that we're ready to move on
	cli_ready();

	//wait for keyboard controller thread to end
	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	return NULL;
	error: 
		stoptui();
		return NULL;
}

//--END starttui()---------------------------

/* stops running the tui interface */
void stoptui()
{
	debug("stopping tui...");
	//stop all managers
	stopwinmgr();
	stopkeyctlr();

	//gracefully wait till the managers have stopped....
	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	//free all the data we've used.
	__free_winstructs();	
	endwin();
}
