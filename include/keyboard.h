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
void* startkeyctlr(void* window);

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
