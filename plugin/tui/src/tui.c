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
#include <sys/time.h>

#include "dbg.h"
#include "keyboard.h"
#include "tui.h"
#include "winmgr.h"

//--keyboard------------------------------
static pthread_t kbthread;

//buffer used for holding printed keys 
#define PBUFFSIZE 1000
static char printbuff[PBUFFSIZE+1];
static size_t printbuff_ind = 0;

static char cmdbuff[PBUFFSIZE];
//--END keyboard---------------------------

//--windows------------------------------
static pthread_t winthread;
static char running = 1;
//--END windows---------------------------

//--starttui()------------------------------

//helper function for starttui
/* initializes all window structures for screen display*/
int __init_winstructs()
{
	int error_code = 0;

	//initialize menus 
	error_run(!__init_menubar(), error_code = 1);
	//initialize command bar
	error_run(!__init_CMDBAR(), error_code = 2);
	//initialize display window
	error_run(!__init_DISPWIN(), error_code = 3);

	return 0;

	error:
		return error_code;
}

//--TESTING------------------------------
void getlastcmd(char* buff)
{
	strcpy(buff,cmdbuff);
	//erase the last inputs

	size_t i;
	int y,x;
	WINDOW* curr_win = getkeywin();

	//clear the curr_win
	getyx(curr_win,y,x);
	for (i = 0; i <= printbuff_ind; i++)
		mvwprintw(curr_win,y,x-i," ");

	wmove(curr_win,y,x-printbuff_ind);
	wnoutrefresh(curr_win);
	printbuff_ind = 0;
}

//fetches the command from the window and does some cleaning on the screen
int readprintbuff()
{
	WINDOW* curr_win = getkeywin();
	//copy the printbuff into the cmd buff
	strcpy(cmdbuff,printbuff);

	//print the current print buffer
//	printdisp(printbuff);

	//erase the last inputs
	size_t i;
	int y,x;
	getyx(curr_win,y,x);
	for (i = 0; i <= printbuff_ind; i++)
		mvwprintw(curr_win,y,x-i," ");

	wmove(curr_win,y,x-printbuff_ind);
	wnoutrefresh(curr_win);
	printbuff_ind = 0;
}

//call doupdate ever so often instead of after everytime we do something ?
void updatewins()
{
	struct timespec tm_intv;
	struct timespec tm_left = {0};
	tm_intv.tv_sec = 0;
	tm_intv.tv_nsec = DOUPDATE_INTERVAL;

	while(running)
	{
		//run update not so often
		nanosleep(&tm_intv,&tm_left);
		doupdate();
	}
}
//--END TESTING---------------------------

//the default action to take place when a key is pressed
//default action is to print the key pressed to the current window
int default_event()
{
	WINDOW* curr_win = getkeywin();
	waddch(curr_win,curr_key); 
	wnoutrefresh(curr_win);
	printbuff[printbuff_ind] = curr_key;
	printbuff[printbuff_ind+1] = '\0';
	if(printbuff_ind == PBUFFSIZE-1)
		printbuff_ind = 0;
	else
		printbuff_ind++;
}

//helper function for __add_default_keys
/* sets up the keyevent list */
void initkeyevents()
{
	size_t i;
	for (i = 0; i < NEVENTS; i++)
	addkeyevent(i,&default_event);
}

//helper function for starttui
/* sets up the default key events */
int __add_default_keys()
{
	initkeyevents();
	printbuff[PBUFFSIZE] = '\0';

	//--default events------------------------------
	//ESC closes the tui
	check(!addkeyevent(ESC_KEY, (event_func_type)stoptui),"failed to add event for key: %i",ESC_KEY); 

	//ENTER closes
//	check(!addkeyevent(ENTER_KEY, (event_func_type)readprintbuff),"failed to add event for key: %i",ENTER_KEY);

	//we want backspacing
	//check_expr(addkeyevent(KEY_BACKSPACE,(event_func_type)stoptui),0,"failed to add key event");

	//--END default events---------------------------
	return 0;

	error:
		return 1;
}

/* sets up and starts running the tui */
void* starttui(void* null)
{
	//vars
	int error_code = 0;

	//init screen
	initscr();

	//create window structs
	check_expr(__init_winstructs(),0,"Failed to create windows. aborting");

		//start the keyboard controller
	log_attempt("Starting keyboard controller");
	error_run(!(error_code = pthread_create(&kbthread,NULL,(void* (*)(void*))startkeyctlr,CMDBAR)), log_failure("Could not start keyboard controller: %i",error_code));
	log_success();
	
	log_attempt("Starting window refresher");
	error_run(!(error_code = pthread_create(&winthread,NULL,(void* (*)(void*))updatewins,NULL)), log_failure("Could not start window refresher"));
	log_success();

	//add default key events
	__add_default_keys();

	//move the cursor to the CMDBAR
	wmove(CMDBAR,1,0);

	//wait for keyboard controller thread to end
	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	return NULL;
	error: 
		stoptui();
		return NULL;
}

int getstatus()
{
	return running;
}

//--END starttui()---------------------------

//--stoptui()------------------------------

/* stops running the tui interface */
int stoptui()
{
	debug("stopping tui...");
	running = 0;
	stopkeyctlr();

	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	__free_winstructs();	
	endwin();
	return 0;
}

//--END stoptui()---------------------------
