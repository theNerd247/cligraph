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

/**
 * @file winmgr.h 
 *
 * @brief macros and functions for the window manager.
 *
 * These functions are not plugin safe unless otherwise specified.
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
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
 * @brief sets the display window.
 *
 * @param win - the window to use for the display window
 *
 * @return int - error code: 0 - no error; 1 - error
 * 
 */
int setdispwin(WINDOW* win);

/**
 * @brief returns the current display window.
 *
 * This is a plugin safe function; call it as often as you like!
 * 
 * @return WINDOW* - the current display window
 * 
 */
WINDOW* getdispwin();

/**
 * @brief returns the command bar window.
 *   
 * @return WINDOW* - the command bar
 * 
 */
WINDOW* getcmdbar();

/**
 * @brief initializes all default window structures. 
 *
 * This function does perform wnoutrefresh() calls, but the actuall displaying
 * of the windows is performed by startwinmgr() .
 * 
 * @return int - 0: no error, 1: something whent wrong.
 * 
 */
int __init_winstructs();

/**
 * @brief gracefully frees all the memory used by the windows the window manager
 * uses.
 * 
 * @bug If the function fails then a seg-fault may occure.
 *
 * @return void - 
 * 
 */
void __free_winstructs();

/**
 * @brief starts the window manager. 
 *
 * This function is called from starttui() and must call tui_ready() in order
 * for the tui to finish loading.
 *
 * @param null - 
 *
 * @return void* - nothing
 * 
 */
void* startwinmgr(void* null);

/**
 * @brief gracefully stops running the window manager.
 *
 * This function: frees all memory used by the window manager and stops running
 * the manager's controller loop.
 * 
 * @return void - 
 * 
 */
void stopwinmgr();

#endif  //end __WINMGR
