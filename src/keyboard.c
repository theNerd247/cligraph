/*
 * keyboard.c is part of cligraph.
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
 * keyboard.c 
 *
 * AUTHOR: Noah Harvey
 * 
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: function definitions of prototypes in keyboard.h
 */

#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#define keyboard_c
#include "keyboard.h"
#include "tui.h"
#define NDEBUG
#include "dbg.h"

/* stores all keyboard events stored as an array*/
pthread_mutex_t events_mutex;
static event_func_type events[NEVENTS];

/* stores the current window the keyboard manager is to use */
static pthread_mutex_t keywin_mutex;
static WINDOW* curr_keywin;

/* the state of the keyboard controller */
static unsigned char run_ctrlr = 0;

int addkeyevent(int key, event_func_type func)
{
	check(func,"Could not set event for key: %i (function invalid)",key);
	check(key >= 0 && key < NEVENTS, "Could not set event for key: %i (key invalid)",key);

	lock(events_mutex);
	events[key] = func;
	unlock(events_mutex);
	return 0;

	error:
		return 1;
}

void removekeyevent(int key)
{
	check(key >= 0 || key < NEVENTS, "Could not remove event for key: %i (key invalid)",key);
	lock(events_mutex);
	events[key] = NULL;
	unlock(events_mutex);
	error: 
		return;
}

int setkeywin(WINDOW* win)
{
	if(!win) return 1;		

	lock(keywin_mutex);
	curr_keywin = win;

	//set keyboard properties
	cbreak(); //don't wait for a new line character
	noecho(); //print what we type
	nodelay(curr_keywin,FALSE); //wait for a character input
	keypad(curr_keywin,TRUE); //allow for keypad stuff

	unlock(keywin_mutex);

	return 0;
}

WINDOW* getkeywin()
{
	WINDOW* win;
	lock(keywin_mutex);
	win = curr_keywin;
	unlock(keywin_mutex);
	return win;
	
}

void* startkeyctlr(void* win)
{
	//initialize mutexes
	pthread_mutex_init(&events_mutex, NULL);	
	pthread_mutex_init(&keywin_mutex, NULL);

	check(!setkeywin((WINDOW*)win),"window was null");
	
	tui_ready();

	//set the controller state to running and start capturing keyboard input
	run_ctrlr = 1;

	int curr_key;
	while(run_ctrlr)
	{
		lock(keywin_mutex);
		curr_key = wgetch(curr_keywin);
		unlock(keywin_mutex);

		if(curr_key != ERR)
		{
			//if the key is valid call its event function
			lock(events_mutex);
			events[curr_key](curr_key);
			debug("Key pressed: %i -- %c",curr_key, curr_key);
			unlock(events_mutex);
		}
	}

	error: 
		return NULL;
}

void stopkeyctlr()
{
	//stop running the controller 
	run_ctrlr = 0;

	pthread_mutex_destroy(&events_mutex);
	pthread_mutex_destroy(&keywin_mutex);
}
