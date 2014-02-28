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
#include <llist.h>

#define keyboard_c
#include "cligraph.h"
#include "keyboard.h"
#include "tui.h"
#define NDEBUG
#include "dbg.h"

/* stores all keyboard events stored as an array*/
pthread_mutex_t events_mutex;
static event_func_type events[NEVENTS];

/* structures for the cmdevents */
typedef struct __cmd_event_st
{
	WINDOW* cmdwin;
	cmd_func_type cmdfunc;
} CMDEVENT;

/* stores the command windows and events */
static LList* cmdevents;
static pthread_mutex_t cmdevent_mutex;

/* stores the current window the keyboard manager is to use */
static pthread_mutex_t keywin_mutex;
static WINDOW* curr_keywin;

/* the state of the keyboard controller */
static unsigned char run_ctrlr = 0;

int __do_nothing(int key)
{
	debug("__do_nothing called by key: %i",key);
	return 0; //as the name indicates this function does nothing.
}

int addkeyevent(int key, event_func_type func)
{
	check(func,"Could not set event for key: %i (function invalid)",key);
	check(key >= 0 && key < NEVENTS, "Could not set event for key: %i (key invalid)",key);

	lock(events_mutex);
	//if the key is before default check it....
	switch(key)
	{
		case ESC_KEY:
		case ENTER_KEY:
		case KEY_F(1):
		case KEY_F(2):
		case KEY_F(3):
		case KEY_F(4):
		case KEY_F(5):
		case KEY_F(6):
		case KEY_F(7):
		case KEY_F(8):
		case KEY_F(9):
		case KEY_F(10):
		case KEY_F(11):
		case KEY_F(12):
			goto check;
		default:
			events[key] = func;
	}	

	//...and see if the key hasn't already been set, if it has not then set it.
	check:
		if(events[key] == __do_nothing || events[key] == NULL)
			events[key] = func;

	unlock(events_mutex);
	return 0;

	error:
		return 1;
}

int removekeyevent(int key)
{
	return addkeyevent(key, __do_nothing);
}

/*TODO: this function needs to be as fast as possible (maybe a redesign is
  needed for mapping of windows to functions */
int sendcmd(int key)
{
	WINDOW* win = getkeywin();

	size_t find_win(void* data)
	{
		CMDEVENT* event = (CMDEVENT*)data;
		if(event->cmdwin == win)
		{
			event->cmdfunc(win);
			return 1;
		}

		return 0;
	}

	lock(cmdevent_mutex);
	llapply(cmdevents,find_win);
	unlock(cmdevent_mutex);

	return 0;
}

int addcmdevent(WINDOW* win, cmd_func_type func)
{
	check(win, "win was null");
	check(func, "func was null");
	
	lock(cmdevent_mutex);
	char exists = 0;

	size_t __check_exists(void* data)
	{
		CMDEVENT* event = (CMDEVENT*)data;
		if(event->cmdwin == win)
		{
			exists = 1;
			event->cmdfunc = func;
			return 1;
		}
		return 0;
	}
	
	llapply(cmdevents, __check_exists);
		
	if(!exists)
	{
		CMDEVENT* newevent = (CMDEVENT*)malloc(sizeof(CMDEVENT));
		newevent->cmdwin = win;
		newevent->cmdfunc = func;
		llappend(cmdevents, newevent);
	}
	unlock(cmdevent_mutex);
	return 0;

	error:
		return 1;
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
	pthread_mutex_init(&cmdevent_mutex, NULL);

	check(!setkeywin((WINDOW*)win),"window was null");

	//init the cmdevents
	cmdevents = llnew(); 
	check(cmdevents, "could not create command events");

	//set the default keys
	size_t i;
	for (i = 0; i < NEVENTS; i++)
	{
		addkeyevent(i,__do_nothing);
	}

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
	pthread_mutex_destroy(&cmdevent_mutex);
	lldestroy(cmdevents);
}

