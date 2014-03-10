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
#include <regex.h>
#include <llist.h>
#include <str.h>

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
static int cmdbuff_index;//used by printable_event to place characters inside the cmdbuff

//the stack of command bar functions
static LList* cmdbarfuncs;
typedef struct 
{
	char* funcname;
	cmdbarevent_func funcref;
} CMDBARFuncNode;

int resetdispwin()
{
	return wclear(getdispwin());
}

int printdispwin(char* buff)
{
	wprintw(getdispwin(),"\n");
	wprintw(getdispwin(),"%s",buff);
	wnoutrefresh(getdispwin());
	return 0;
}

//--starttui()------------------------------

int addcmdbarfunc(cmdbarevent_func func, char* funcalias)
{
	CMDBARFuncNode* newNode = (CMDBARFuncNode*)malloc(sizeof(CMDBARFuncNode));
	newNode->funcname = funcalias;
	newNode->funcref = func;
	
	return llappend(cmdbarfuncs,(void*)newNode) == NULL ? 1 : 0;
}

void removecmdbarfunc(char* funcalias)
{
	CMDBARFuncNode* node;
	char removefunc(void* data)
	{
		//if the node matches a name free its memory and don't keep it
		node = (CMDBARFuncNode*)data;
		if(!strcmp(funcalias,node->funcname))
		{
			free(node->funcname);
			free(node->funcref);
			free(node);
			return 0;
		}
		return 1;
	}	

	LList* temp =  cmdbarfuncs;
	cmdbarfuncs = llfilter(cmdbarfuncs,&removefunc);
	lldestroy(temp);
}

cmdbarevent_func getcmdbarfunc(char* alias)
{
	CMDBARFuncNode* node;
	cmdbarevent_func func = NULL;
	size_t getfunc(void* data)
	{
		//if the node matches a name free its memory and don't keep it
		node = (CMDBARFuncNode*)data;
		if(!strcmp(alias,node->funcname))
		{
			func = node->funcref;
			return 1;
		}
		return 0;
	}	
	
	llapply(cmdbarfuncs,&getfunc);

	return func;
}

#define FUNCPAT "^([-_a-zA-Z0-9]*)\\({0,1}(.*)\\){0,1}"
//helper function for cmbdbar_cmdevent
/** 
 * @brief parses the function that is being called via the cmdbar 
 * 
 * @param raw - raw string from cmdbar

 * @return char*
 */
void parsefuncname(char* raw, char* funcbuff, char* argbuff)
{
	regex_t regt;
	regoff_t match;
	char temp[255];
	regmatch_t matches[3];

	match = regcomp(&regt,FUNCPAT,REG_EXTENDED);
	check((match == 0), "failed to compile regex_t: %i",match);

	match = regexec(&regt,raw,3,matches,0);
	check((match == 0),"Failed to match expression error code: %i",match);
	check((matches[1].rm_so != -1),"No valid index for substring");

	//grab the function name
	check(strsub(raw,matches[1].rm_so,matches[1].rm_eo-1,funcbuff),"could not make substring");
	//grab the arguments to function
	//dont check for arguments to given function
	strsub(raw,matches[2].rm_so,matches[2].rm_eo-2,argbuff);

	return;

	error:
		regerror(match,&regt,temp,255);
		log_err("%s",temp);
}

/** this is the cmd_event_func for the command bar
 *
 * Its main job is to handle the interface actions (clearing the cmdbar,
 * fetching the input buffer, looking up and executing the function, etc..)
 */
int cmdbar_cmdevent(WINDOW* cmdbar)
{
	//find the command and send it
	char funcname[255] = "\0";
	char args[255] = "\0";
	cmdbarevent_func func = NULL;
	size_t i;

	parsefuncname(cmdbuff,funcname,args);	
	if(funcname[0] == '\0') goto clear;

	//call the function if it exists
	func = getcmdbarfunc(funcname);
	if(func != NULL)
		func(args);

	clear: 
		//clear the cmdbar and the cmdbuffer
		for (i = 0; i <= cmdbuff_index; i++)
			cmdbuff[i] = '\0';
	
		cmdbuff_index = 0;
	
		//clear the command bar
		wmove(cmdbar,1,0);
		wclrtoeol(cmdbar);
		wnoutrefresh(cmdbar);

	return 0;
}

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

int delchar(int curr_key)
{
	if(cmdbuff_index != 0) cmdbuff_index--;

	cmdbuff[cmdbuff_index] = ' ';

	WINDOW* curr_win = getcmdbar();
	mvwprintw(curr_win,1,0,"%s",cmdbuff);
	wmove(curr_win,1,cmdbuff_index);
	wnoutrefresh(curr_win);
	return 0;
}

//quit alias command for command bar
void quitcmd(char* nothing)
{
	stoptui();
}

int switchmenu(int key)
{
	resetdispwin();
	return setdispwin(key-KEY_F0);
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
	//ESC closes the tui -- and any alias
	check_error(!addkeyevent(ESC_KEY, (keyevent_func)stoptui)); 
//	check_error(!addcmdbarfunc(&quitcmd,"quit"));

	//__sendcmd is defined in keyboard.c
	check_error(!addkeyevent(ENTER_KEY, __sendcmd));
	check_error(!addcmdevent(getcmdbar(),cmdbar_cmdevent));

	//backspace key
	check_error(!addkeyevent(KEY_BACKSPACE,delchar));
	//--END other default events---------------------------

	//function keys
	for (i = 1; i < NMENUS+1; i++)
	{
		check_error(!addkeyevent(KEY_F(i),switchmenu));
	}

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

	//init the cmdbarfuncs list
	cmdbarfuncs = llnew();
	check(cmdbarfuncs != NULL, "failed to init cmdbarfuncs....aborting");
	
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
		cli_ready();
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
