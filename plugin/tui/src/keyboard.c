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

#include "keyboard.h"
#include "tui.h"
#define NDEBUG
#include "dbg.h"

/* stores all keyboard events stored as an array*/
#define NEVENTS 634
pthread_mutex_t events_mutex = PTHREAD_MUTEX_INITIALIZER;
event_func_type events[NEVENTS];

//the current window the kybdctrlr is working with
WINDOW* curr_win;

//the state of the keyboard controller
unsigned char run_ctrlr = 1;

//current key that has been pressed
int curr_key;

//the default action to take place when a key is pressed
//default action is to print the key pressed to the current window
int default_event()
{
	waddch(curr_win,curr_key);
}

int addkeyevent(int key, event_func_type func)
{
	check(func,"Could not set event for key: %i (function invalid)",key);
	check(key >= 0 && key < NEVENTS, "Could not set event for key: %i (key invalid)",key);
//	check(events[key] != &default_event, "Could not set event for key: %i (event exists)",key);

//	debug("ADDING KEY EVENT FOR KEY: %i",key);
	pthread_mutex_lock(&events_mutex);
	events[key] = func;

	pthread_mutex_unlock(&events_mutex);
	return 0;

	error:
		return 1;
}

void removekeyevent(int key)
{
	check(key >= 0 || key < NEVENTS, "Could not remove event for key: %i (key invalid)",key);
	events[key] = default_event;
	error: 
		return;
}

int setkeywin(WINDOW* win)
{
	if(!win) return 1;
	curr_win = win;
	//set keyboard properties
	nodelay(curr_win,FALSE); //wait for a character input
	keypad(curr_win,TRUE); //allow for keypad stuff

	return 0;
}

//helper function for startkeyctlr
/* sets up the keyevent list */
void initkeyevents()
{
	size_t i;
	for (i = 0; i < NEVENTS; i++)
	addkeyevent(i,&default_event);
}

int startkeyctlr(void* win)
{
	curr_win = (WINDOW*)win;
	check(curr_win,"window was null");

	//keyboard config
	cbreak(); //don't wait for a new line character
	noecho(); //print what we type
	nodelay(curr_win,FALSE); //wait for a character input
	keypad(curr_win,TRUE);

	//current key pressed
	run_ctrlr = 1;

	while(run_ctrlr)
	{
		if((curr_key = wgetch(curr_win)) != ERR)
		{
			debug("KEY PRESSED -- INT: %i CHAR: %c",curr_key,curr_key);	
			events[curr_key]();
		}
	}

	return 0;
	error: 
		return 1;
}

void stopkeyctlr()
{
	run_ctrlr = 0;
}
