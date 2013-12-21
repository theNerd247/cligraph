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

#include "dbg.h"
#include "tui.h"
#include "keyboard.h"
#include "winmgr.h"

//--THREADS------------------------------
pthread_t kbthread;
pthread_t winthread;

pthread_mutex_t wait_mutex;
pthread_cond_t wait_cond;
unsigned char wait_sig;
//--END THREADS---------------------------

//--keyboard------------------------------

//buffer used for holding printed keys 
#define PBUFFSIZE 1000
static char printbuff[PBUFFSIZE+1];
static size_t printbuff_ind = 0;

static char cmdbuff[PBUFFSIZE];
//--END keyboard---------------------------

//--windows------------------------------
//--END windows---------------------------

/* status of the tui manger */
static char running = 1;
//--starttui()------------------------------

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
	return 0;
}

//--END TESTING---------------------------

//the default action to take place when a key is pressed
//default action is to print the key pressed to the current window
int default_event(int curr_key)
{
	WINDOW* curr_win = getkeywin();
	waddch(curr_win,curr_key); 
	wnoutrefresh(curr_win);
	return 0;
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
	
	pthread_mutex_init(&wait_mutex, NULL);
	pthread_cond_init(&wait_cond, NULL);

	//init screen
	initscr();

	//start the window manager
	log_attempt("Starting window refresher");
	error_run(!(error_code = pthread_create(&winthread,NULL,startwinmgr,NULL)), log_failure("Could not start window refresher"));
	//wait till the winmgr says it's ok to move on
	tui__wait();
	log_success();


	//start the keyboard controller
	log_attempt("Starting keyboard controller");
	WINDOW* cmdbar = getcmdbar();
	error_run(!(error_code = pthread_create(&kbthread,NULL,startkeyctlr,(void*)cmdbar)), log_failure("Could not start keyboard controller: %i",error_code));
	//wait till the kbdmgr says it's ok to move on
	tui__wait();
	log_success();
	
	//add default key events
	__add_default_keys();

	//move the cursor to the CMDBAR
	wmove(getcmdbar(),1,0);

	//wait for keyboard controller thread to end
	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	return NULL;
	error: 
		stoptui();
		return NULL;
}

//--END starttui()---------------------------

//--stoptui()------------------------------

/* stops running the tui interface */
int stoptui()
{
	debug("stopping tui...");
	running = 0;
	stopwinmgr();
	stopkeyctlr();

	pthread_join(kbthread,NULL);
	pthread_join(winthread,NULL);

	__free_winstructs();	
	endwin();
	return 0;
}

//--END stoptui()---------------------------
