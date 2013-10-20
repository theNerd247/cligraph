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

#include <string.h>
#include <ncurses.h>
#include <stdlib.h>

#include "tui.h"
#include "dbg.h"

/* menus for the toolbar */
#define NMENUS 6
MENU* menus[NMENUS];
const char* menu_titles[NMENUS] = {"Math","Functions","Tables","Graphs","Settings","Plugin"}; //titles of the table menu

//helper function for __init_winstructs
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

//helper function for starttui
/* initializes all window structures for screen display*/
int __init_winstructs()
{
	int error_code = 0;
	//initialize menus 
	size_t i;
	for (i = 0; i < NMENUS; i++)
	{
		error_run(menus[i] = __init_menu(7,COLS/6,0,i*(COLS/6),menu_titles[i]), error_code =1);
	}
	//initialize displays

	//initialize command bar
	
	return 0;

	error:
		return error_code;
}

void* starttui(void* null)
{
	//vars
	int error_code = 0;
	//perform all ncurses init stuff here
	initscr();

	//keyboard config
	cbreak();
	noecho();
	refresh();
	keypad(stdscr, TRUE); //we want to enable F1-F12, etc...

	//create window structs
	error_code = __init_winstructs();

	//display the windows
	doupdate();

	//process keyboard events
	while(getch() != 'q')
		;

	error: 
		stoptui();
		return NULL;
}

void stoptui()
{
	endwin();
}
