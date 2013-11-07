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

#include "dbg.h"
#include "winmgr.h"

//window titles
static const char* __menu_titles[NMENUS] = {"Math","Functions","Tables","Graphs","Settings","Plugin"}; //titles of the table menu

//helper function for __init_winstructs
/* initializes the window to contain plugin displays */
char __init_DISPWIN()
{
	check(DISPWIN = newwin(DISP_YS,DISP_XS,DISP_YP,DISP_XP), "Failed to create DISPWIN");
	
	//keyboard config
	keypad(DISPWIN, TRUE);

	//cursor config
	leaveok(DISPWIN, FALSE);

	//borders 
	mvwvline(DISPWIN,0,0,ACS_VLINE,DISP_YS);
	mvwvline(DISPWIN,0,DISP_XS-1,ACS_VLINE,DISP_YS);

	//start cursor at the top of the window 
	wmove(DISPWIN,0,1);
	wnoutrefresh(DISPWIN);

	return 0;
	
	error: 
		return 1;
}

//helper function for __init_menus
/* initializes all the menus at the top of the screen */
//assumes all arguments are valid arguments
MENU* __init_menu(size_t ysize, size_t xsize, size_t ypos, size_t xpos, const char* title)
{
	//vars 
	int error = 0;

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

//helper function for __init_winstructs
/* initializes CMDBAR */
char __init_CMDBAR()
{
	//init window
	check(CMDBAR = newwin(CMD_YS,CMD_XS,CMD_YP,CMD_XP),"Failed to create command bar");

	//draw the top and bottom borders
	mvwhline(CMDBAR,0,0,ACS_HLINE,CMD_XS);
	mvwhline(CMDBAR,CMD_YS-1,0,ACS_HLINE,CMD_XS);

	//we want controll over the cursor
	leaveok(CMDBAR, FALSE);

	//apply updates and return	
	wnoutrefresh(CMDBAR);
	return 0;

	error:
		return 1;
}

//helper function for __free_winstructs
/* frees all the menus and the data related to them */
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
	wclear(DISPWIN);
	delwin(DISPWIN);

	//free the CMDBAR
	wclear(CMDBAR);
	delwin(CMDBAR);
}
