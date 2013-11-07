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

//--GLOBALS------------------------------
//--menus------------------------------
#define NMENUS 6 
#define MENUWIDTH (COLS/NMENUS)
MENU* menus[NMENUS];
//--END menus---------------------------

//--cmdbar------------------------------ #define CMD_YS 3 #define CMD_XS COLS
#define CMD_YS 3
#define CMD_XS COLS
#define CMD_YP LINES-3
#define CMD_XP 0
#define CMD_GRAB_Y 1 //the line to grab text from the cmdbar
WINDOW* CMDBAR;
//--END cmdbar---------------------------

//--display window------------------------------
#define DISP_YS LINES-5
#define DISP_XS COLS
#define DISP_YP 2
#define DISP_XP 0
WINDOW* DISPWIN;
//--END display window---------------------------

//--END GLOBALS---------------------------

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

/*
 * FUNCTION: __init_DISPWIN
 * 
 * DESCRIPTION: initialize the display window
 *
 * PARAMETERS: size_t ysize, size_t xsize, size_t ypos, size_t xpos
 *
 * RETURNS: char - error code: 0 - no error; 1 - something bad happened 
 */
char __init_DISPWIN();

/*
 * FUNCTION: __init_menubar
 * 
 * DESCRIPTION: creates the menubar
 *
 * PARAMETERS: 
 *
 * RETURNS: char - error code: 0 - no error; 1 - something bad happened
 * 
 */
char __init_menubar();

/*
 * FUNCTION: __init_CMDBAR
 * 
 * DESCRIPTION: intializes the command bar
 *
 * PARAMETERS: 
 *
 * RETURNS: char - error code: 0 - no error; 1 - something bad happened
 * 
 */
char __init_CMDBAR();

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

#endif  //end __WINMGR
