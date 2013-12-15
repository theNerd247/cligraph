/*
 * keyboard.h is part of cligraph.
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
 * keyboard.h 
 *
 * AUTHOR: Noah Harvey
 * 
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: function prototypes for the keyboard controller
 */

#include <ncurses.h>
#include <pthread.h>

#include "winmgr.h"

#ifndef __KEYBOARDCTL
#define __KEYBOARDCTL

//the maximum number of keys 
#define NEVENTS 634

//--globals------------------------------
//type for an keyevent-function
typedef int (*event_func_type)(void);

//the last key that was pressed
static int curr_key;
//--END globals---------------------------

/*
 * FUNCTION: addkeyboard event
 * 
 * DESCRIPTION: adds a keyboard event for the keyboard controller. This allows
 * other plugins to perform operations when a keyboard is pressed
 *
 * PARAMETERS: int key, event_func_type func
 *
 * RETURNS: int - error code: 0 - no error, assume event added; 1 - event alread
 * exists; 2 - some other error happened
 * 
 */
int addkeyevent(int key, event_func_type func);

/*
 * FUNCTION: removekeyevent
 * 
 * DESCRIPTION: removes the key event that is paired with the given key
 *
 * PARAMETERS: int key
 *
 * RETURNS: int - error code: 0 - no error code; 1 - invalid key
 * 
 */
void removekeyevent(int key);

/*
 * FUNCTION: setkeywin
 * 
 * DESCRIPTION: sets the window the keyboard controller fetches characters from
 *
 * PARAMETERS: WINDOW* win
 *
 * RETURNS: int - error code: 0 - no error; 1 - something  whent wrong
 * 
 */
int setkeywin(WINDOW* win);

/**
 * fetches the pointer to the window that is currently held by the keyboard
 * controller.
 *
 * @return WINDOW* - window pointer. NULL is returned if an error occurs
 * 
 */
WINDOW* getkeywin();

/*
 * FUNCTION: startkeyctlr
 * 
 * DESCRIPTION: starts the keycontrolling thread
 *
 * PARAMETERS: WINDOW* window - window to run the ctrller on (typically this
 * will be the cmdbar)
 *
 * RETURNS: void* - absolutely nothing so far
 * 
 */
int startkeyctlr(void* window);

/*
 * FUNCTION: stopkeyctlr
 * 
 * DESCRIPTION: stops the key controller in a graceful way. This is a safe
 * function to call in need of immediate shutdown
 *
 * PARAMETERS: 
 *
 * RETURNS: void - nothing.
 * 
 */
void stopkeyctlr();

#endif 
