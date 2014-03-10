/*
 * winmgr.c is part of cligraph.
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
 * winmgr.c 
 *
 * AUTHOR: Noah Harvey
 * 
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: definition file for winmgr.h
 */

#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "dbg.h"

//define variables located in winmgr.c
#include "tui.h"
#define winmgr_c
#include "winmgr.h"

//--WINSTRUCTS------------------------------
static MENU* menus[NMENUS];
static WINDOW* CMDBAR;
static WINDOW* DISPWINS[NMENUS];
static WINDOW* DISPWIN;
static WINDOW* DISPWIN_ORIG;

static pthread_mutex_t cmdbar_mutex;
static pthread_mutex_t dispwin_mutex;
//--END WINSTRUCTS---------------------------

//window titles
static const char* __menu_titles[NMENUS] = {"Math","Functions","Tables","Graphs","Settings","Plugin"}; //titles of the table menu

/* status of window manager */
static unsigned char running = 0;

/* helper function for __init_winstructs */
char __init_DISPWIN()
{
	lock(dispwin_mutex);
	DISPWIN_ORIG = newwin(DISP_YS,DISP_XS,DISP_YP,DISP_XP);
	size_t i;
	for (i = 0; i < NMENUS; i++)
	{
		check(DISPWINS[i] = subwin(DISPWIN_ORIG,DISP_YS-2,DISP_XS-2,DISP_YP+1,DISP_XP+1), "Failed to create DISPWIN");
	
		//keyboard config
		keypad(DISPWINS[i], TRUE);
	
		//cursor config
		leaveok(DISPWINS[i], FALSE);
	}

	//borders 
	mvwvline(DISPWIN_ORIG,0,0,ACS_VLINE,DISP_YS);
	mvwvline(DISPWIN_ORIG,0,DISP_XS-1,ACS_VLINE,DISP_YS);
	wnoutrefresh(DISPWIN_ORIG);

	//init with the math dispwin
	DISPWIN = DISPWINS[0];

	//start cursor at the top of the window 
	wmove(DISPWIN,0,1);
	wnoutrefresh(DISPWIN);
	
	unlock(dispwin_mutex);

	return 0;
	
	error: 
		return 1;
}

/* helper function for __init_menus */
//assumes all arguments are valid arguments
MENU* __init_menu(size_t ysize, size_t xsize, size_t ypos, size_t xpos, const char* title)
{
	//init new menu
	MENU* menu = new_menu(NULL);
	check(menu, "Failed to create menu: %s", title);

	//init new container window
	WINDOW* container = newwin(ysize,xsize,ypos,xpos);
	check(container, "Failed to create container for menu: %s", title);

	//enable the keypad for the windows
	keypad(container, TRUE);

	//we want controll over the cursor
	leaveok(container, FALSE);

	//draw the border and title
	mvwhline(container,1,0,ACS_HLINE,xsize);
	mvwprintw(container,0,(xsize/2)-strlen(title)/2,title);
	wnoutrefresh(container);

	//set the windows and subwindows
	check_expr(set_menu_win(menu, container), E_OK, "Failed to set window for\
			menu: %s",title); 
	check_expr(set_menu_sub(menu,derwin(container,ysize-2,xsize-2,2,1)), E_OK,"F\
			ailed to set window for menu:	%s", title);

	return menu;

	error: 
		return NULL;
}

/* helper function for __init_winstructs */
char __init_menubar()
{
	size_t i;
	for (i = 0; i < NMENUS; i++)
	{
		int width = i < NMENUS-1 ? MENUWIDTH : COLS-(i*MENUWIDTH);
		check(menus[i] = __init_menu(7,width,0,i*MENUWIDTH,__menu_titles[i]), "Failed  to initialize menu: %s",__menu_titles[i]);
	}

	return 0;
	error:
		return 1;
}

/* helper function for __init_winstructs */
char __init_CMDBAR()
{
	lock(cmdbar_mutex);
	//init window
	check(CMDBAR = newwin(CMD_YS,CMD_XS,CMD_YP,CMD_XP),"Failed to create command bar");

	//draw the top and bottom borders
	mvwhline(CMDBAR,0,0,ACS_HLINE,CMD_XS);
	mvwhline(CMDBAR,CMD_YS-1,0,ACS_HLINE,CMD_XS);

	//we want controll over the cursor
	leaveok(CMDBAR, FALSE);

	//apply updates and return	
	wnoutrefresh(CMDBAR);

	unlock(cmdbar_mutex);
	return 0;

	error:
		return 1;
}

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

/* helper function for __free_winstructs */
//assumes arguments are valid
void __free_menu(MENU* menu)
{
	wclear(menu_win(menu));

	ITEM** items = menu_items(menu);
	if(items == NULL)
		goto free;

	size_t i;
	for (i = 0; i < 5; i++)
		free_item(items[i]);
		free(items);

	free:
		free_menu(menu);
}

void __free_winstructs()
{
	//free the menus
	size_t i;
	for (i = 0; i < NMENUS; i++)
		__free_menu(menus[i]);

	//free the displays
	for (i = 0; i < NMENUS; i++)
	{
		wclear(DISPWINS[i]);
		delwin(DISPWINS[i]);
	}

	//free the CMDBAR
	wclear(CMDBAR);
	delwin(CMDBAR);
}

WINDOW* getcmdbar()
{
	WINDOW* win = NULL;
	lock(cmdbar_mutex);
	win = CMDBAR;
	unlock(cmdbar_mutex);
	return win;
}

int setdispwin(int menu)
{
	check(!(menu < 0 || menu > NMENUS-1), "invalid menu value");
	lock(dispwin_mutex);
	DISPWIN = DISPWINS[menu];
	wnoutrefresh(DISPWIN);
	unlock(dispwin_mutex);
	return 0;

	error:
		return 1;
}

WINDOW* getdispwin()
{
	WINDOW* win = NULL;
	lock(dispwin_mutex);
	win = DISPWIN;
	unlock(dispwin_mutex);
	return win;
}

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

void* startwinmgr(void* null)
{
	//init mutexes
	pthread_mutex_init(&cmdbar_mutex, NULL);
	pthread_mutex_init(&dispwin_mutex, NULL);
	
	__init_winstructs();
	running = 1;

	tui_ready();

	updatewins();
	return NULL;
}

void stopwinmgr()
{
	running = 0;
	pthread_mutex_destroy(&dispwin_mutex);
	pthread_mutex_destroy(&cmdbar_mutex);
}
