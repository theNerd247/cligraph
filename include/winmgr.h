/*
 * winmgr.h is part of cligraph.
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
 * winmgr.h 
 *
 * AUTHOR: Noah Harvey
 * 
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: manager of the windows 
 */

#include <ncurses.h>
#include <menu.h>

#ifndef __WINMGR
#define __WINMGR

//define have extern exist for all inclusions except for winmgr.c
#ifdef winmgr_c
#define EXTERN 
#else
#define EXTERN extern
#endif

//--GLOBALS------------------------------
//--menus------------------------------
#define NMENUS 6 
#define MENUWIDTH (COLS/NMENUS)
//--END menus---------------------------

//--cmdbar------------------------------ 
#define CMD_YS 3
#define CMD_XS COLS
#define CMD_YP LINES-3
#define CMD_XP 0
#define CMD_GRAB_Y 1 //the line to grab text from the cmdbar

//--END cmdbar---------------------------

//--display window------------------------------
#define DISP_YS LINES-5
#define DISP_XS COLS
#define DISP_YP 2
#define DISP_XP 0
//--END display window---------------------------

//--END GLOBALS---------------------------

/**
 * sets the display window
 *
 * @param win - the window to use for the display window
 *
 * @return int - error code: 0 - no error; 1 - error
 * 
 */
int setdispwin(WINDOW* win);

/**
 * returns a pointer to the current window being used as the display window
 *
 * 
 * @return WINDOW* - the current display window
 * 
 */
WINDOW* getdispwin();

/**
 * returns a pointer to the window that is used for the display bar.
 *
 * 
 * @return WINDOW* - the command bar
 * 
 */
WINDOW* getcmdbar();

/*
 * FUNCTION: fetchchars
 * 
 * DESCRIPTION: fetches the last printed characters in the current window held
 * by the keycontroller at x,y (this only returns up to the first new-line
 * character as it uses mvwgetnstr(). This function removes the characters from
 * the current window
 *
 * PARAMETERS: size_t xpos, size_t ypos, size_t n
 *
 * RETURNS: char* - string contained. Return NULL on error or if no characters
 * were found
 * 
 */
char* fetchchars(size_t xpos, size_t ypos, size_t n);

/**
 * initializes all default window structures. This doesnot actually call for the
 * structures to be displayed.
 *
 * 
 * @return int - error code: 0 - no error; 1 - error occured
 * 
 */
int __init_winstructs();

/*
 * FUNCTION: __free_winstructs
 * 
 * DESCRIPTION: frees all the window structures when done
 *
 * PARAMETERS: 
 *
 * RETURNS: void - never fails
 * 
 */
void __free_winstructs();

/**
 * starts the window manager. Call this method when creating a new thread.
 *
 * @param null - 
 *
 * @return void* - nothing
 * 
 */
void* startwinmgr(void* null);

/**
 * stops running the window manager
 *
 * 
 * @return void - 
 * 
 */
void stopwinmgr();

#endif  //end __WINMGR
