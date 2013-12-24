/*
 * cligraph.c is part of cligraph.
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
 * @file cligraph.h 
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 *
 * header for libcligraph
 */

#include <ncurses.h>

#ifndef __CLIGRAPH
#define __CLIGRAPH

//--PLUGIN CONFIG------------------------------

#define PLUGIN_PATH "plugin"
#define DL_LOAD_FLAG (RTLD_LAZY || RTLD_GLOBAL)

//--END PLUGIN CONFIG---------------------------

//--PLUGINS ------------------------------

/**
 * returns a void pointer to the function in the given plugin 
 *
 * @param plugname - name of the plugin to search
 * @param funcname - name of the function needed
 *
 * @return void* - void pointer to function. NULL is returned if error occurs. 
 * 
 */
void* getfuncref(char* plugname, char* funcname);

/**
 * lets the cligraph controller thread know that the current thread is ready to
 * move on. This is the base system for sequence controll in the program to
 * prevent race conditions. Call this method at the end of a plugin's start
 * function (and before that function enters it's main control loop if the
 * plugin should run in this manner).
 *
 * 
 * @return void - 
 * 
 */
void cli_ready();

//--END PLUGINS ---------------------------

//--SPECIAL KEY CODES------------------------------
#define PRINTKEY_RANGE_MIN 32
#define PRINTKEY_RANGE_MAX 126
#define ESC_KEY 27
#define ENTER_KEY 10
//--END SPECIAL KEY CODES---------------------------

//--KEYBOARD------------------------------
//type for an keyevent-function
typedef int (*event_func_type)(int keycode);

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

//--END KEYBOARD---------------------------

//--COMMANDS------------------------------

/**
 * fetches the currently inputed command and attempts to execute it
 *
 * @param key - the key that was pressed (see event_func_type in keyboard.h)
 *
 * @return void -
 * 
 */
void sendcmd(int key);

//--END COMMANDS---------------------------

#endif 
