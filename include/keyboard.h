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

/**
 * @file keyboard.h 
 *
 * functions used by the keyboard controller. 
 *
 * These functions are not plugin safe unless otherwise specified. If you are
 * looking for keyboard interfaces for plugins then please see cligraph.h
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

#include <ncurses.h>
#include <pthread.h>

#include "winmgr.h"

#ifndef __KEYBOARDCTL
#define __KEYBOARDCTL

/** the number of keyboard events */
#define NEVENTS 634

/**
 * @brief starting function for the keyboard controller.
 *
 * This function is called by starttui() to start a new thread. This function
 * must call the tui_ready() function to signal that it's finished setting up.
 * 
 * @param window - the initial window to use for the keyboard controller.
 * by default this is the command bar.
 *
 * @return void* - NULL
 * 
 */
void* startkeyctlr(void* window);

/**
 * @brief gracefully stops running the keyboard controller.
 * 
 * This function: frees all memory associated to the keyboard controller, an halts the
 * controller loop that runs the keyboard controller.
 *
 * 
 * @return void - 
 * 
 */
void stopkeyctlr();

#endif 
