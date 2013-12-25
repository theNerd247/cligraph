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
 * @brief The cligraph API header file. 
 *
 * These functions are thread safe functions. 
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

#include <ncurses.h>

#ifndef __CLIGRAPH
#define __CLIGRAPH

//--PLUGINS ------------------------------

/**
 * returns a void pointer to the function in the given plugin. 
 *
 * The given plugin that is being called must be loaded before the function can
 * be called. Later there will be a dynamic system to allow for proper plugin
 * dependencies.
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
 * move on. 
 *
 * This is the base system for sequence controll in the program to
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
#define ESC_KEY 27
#define ENTER_KEY 10
//--END SPECIAL KEY CODES---------------------------

//--KEYBOARD------------------------------
/** function type to use for keyboard events */
typedef int (*event_func_type)(int keycode);

/** function type to use for cmd events */
typedef int (*cmd_func_type)(WINDOW* win);

/**
 * @brief maps the given function to the specified key. 
 *
 * This will override any previously defined functions mapped for that key. This
 * function should rarely be used. If you are trying to map a given function to
 * a command then use addcmdevent(). NOTE: Some keys cannot be overriden.
 * Below is a list of these keys: 
 *
 * KeyCode  Key
 *   27     ESC_KEY
 *   10     ENTER_KEY
 *   ??     KEY_F(1-12)
 *
 * @param key - the key to map. The key value must be between 0 and NEVENTS
 * @param func - the function to map. See event_func_type for details.
 *
 * @return int - error code: 0 - no error; 1 - something whent wrong
 * 
 * @todo check for invalid keys
 */
int addkeyevent(int key, event_func_type func);

/**
 * @brief removes the given function mapped to the given key.
 *
 * The function mapped to the key is actually replaced by a "do nothing"
 * function to prevent segfaults. By calling this function, however you can
 * still treat the key as if the function were removed. See adddkeyevent() for a
 * list of keys whose mapping cannot be changed.
 * 
 * @param key - the key to set the function to.
 *
 * @return void  
 * 
 * @todo check for invalid keys
 * @todo replace removed keys to the "do nothing" function.
 */
void removekeyevent(int key);

/**
 * @brief sets the window the keyboard controller should accept keys from.
 * 
 * This should be rarely called. The default window is the command bar at the
 * bottom of the screen.
 *
 * @param win - window to set the screen to.
 *
 * @return int - 0 if no error occurs, 1 if something whent wrong.
 * 
 */
int setkeywin(WINDOW* win);

/**
 * @brief grabs the window the keyboard controller currently holds.
 *
 * @return WINDOW* - window pointer. NULL is returned if an error occurs
 * 
 */
WINDOW* getkeywin();

//--END KEYBOARD---------------------------

//--COMMANDS------------------------------

/**
 * @brief maps the given function to the window for a command event.
 * 
 * The given function will be called if the current window in "focus" is the one
 * held by the keyboard controller and the ENTER_KEY is pressed.
 *
 * @param win - the window to map the function to
 * @param func - the function to add.
 *
 * @return int - 0 if no error, 1 if error occurs
 * 
 * @todo write source 
 */
int addcmdevent(WINDOW* win, cmd_func_type func);

//--END COMMANDS---------------------------

#endif 
