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
static pthread_mutex_t events_mutex = PTHREAD_MUTEX_INITIALIZER;
static event_func_type events[NEVENTS];

//the state of the keyboard controller
static unsigned char run_ctrlr = 1;

static WINDOW* curr_win;

int addkeyevent(int key, event_func_type func)
{
	check(func,"Could not set event for key: %i (function invalid)",key);
	check(key >= 0 && key < NEVENTS, "Could not set event for key: %i (key invalid)",key);

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
	events[key] = NULL;
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

int startkeyctlr(void* win)
{
	curr_win = (WINDOW*)win;
	check(curr_win,"window was null");

	//keyboard config
	cbreak(); //don't wait for a new line character
	noecho(); //print what we type
	nodelay(curr_win,FALSE); //wait for a character input
	keypad(curr_win,TRUE); //allow the keypad

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
