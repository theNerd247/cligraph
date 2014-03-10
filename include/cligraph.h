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
 * @brief cligraph API functions and macros
 *
 * These are the functions to be used for the cligraph API. All plugins should
 * include cligraph.h in order to interface with the program's functionality.
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

#include <ncurses.h>

#ifndef __CLIGRAPH
#define __CLIGRAPH

/** @defgroup Keys Custom Key Mappings not found in ncurses library * @{ 
 */
#define ESC_KEY 27

#define ENTER_KEY 10
/** @} */

/** @defgroup Events cligraph events
 *  @{
 *
 * An event in cligraph is simply a function that adheres to one of the three
 * typedefs for event functions and is called on when an event occures. The
 * three type of events are: 
 *
 * * keyevents - whenever a specific key is pressed
 * * cmdevents - whenver the ENTER key is pressed within a ncurses WINDOW
 * * cmdbarevents - whenever a function is requested through the cmdbar (this is
 * a spcecific set of events for when the cmdevent occures for the cmdbar).
 *
 */

/** @brief function type to use for keyboard events 
 *
 * Typedef to map a keycode to a function. 
 *
 * @param keycode - the keycode (generally the ascii value of the given key)
 * that called the function.
 *
 * @see addkeyevent()
 * @see removekeyevent()
 */
typedef int (*keyevent_func)(int keycode);

/** @brief function type to use for cmd events 
 *
 * Typedef to map an event function to a window for when the command key (ENTER)
 * is pressed.
 *
 * @param win - ncurses WINDOW object that was held last by the keyboard
 * controller when a window event was created. This will be the window that the
 * mapped function is called to.
 *
 * @see addcmdevent()
 */
typedef int (*cmdevent_func)(WINDOW* win);

/** @brief function type for commands to be called through command bar 
 *
 * Typedef to map a given command string (given via the command bar by user) to
 * a function. 
 *
 * @param text - a c-string as arguments that are passed to the mapped function
 * when called. The called function is expected to perform its own parsing if
 * needed
 *
 * @see addcmdbarfunc()
 * @see removecmdbarfunc()
 */
typedef void (*cmdbarevent_func)(char* text);
/** @} */

/**
 * @brief fetches a pointer to the given function held by the given plugin
 *
 * The given plugin that is being called must be loaded before the function can
 * be called. 
 *
 * @param plugname - name of the plugin to search
 * @param funcname - name of the function needed
 *
 * @return void* - void pointer to function. NULL is returned if error occurs. 
 * 
 */
void* getfuncref(char* plugname, char* funcname);

/**
 * @brief let cligraph know that it can stop waiting on the current thread
 *
 * This is the base system for sequence controll in the program to
 * prevent race conditions. Call this method at the end of a plugin's start
 * function (and before that function enters it's main control if it has one).
 * **If this function is not called race conditions may incure and your plugin may
 * not work.**
 * 
 * @return void 
 * 
 */
void cli_ready();

/**
 * @brief sets the window the keyboard controller should accept keys from.
 * 
 * This should be rarely called. The default window is the command bar at the
 * bottom of the screen.
 *
 * @param win - window to set the screen to.
 *
 * @return int - 0 if no error occurs, 1 if error occured.
 * 
 */
int setkeywin(WINDOW* win);

/**
 * @brief fetch the current keywin
 *
 * This function is thread safe, however the expected window returned may change
 * between function call and return
 * 
 * @return WINDOW* - a ncurses WINDOW pointer
 * 
 */
WINDOW* getkeywin();

/**
 * @brief prints a buffer to the current display window.
 *
 * The printing of this buffer will start at coordinates 0,0 by default. This
 * function is useful for being called by a graphing function.
 * 
 * @param buff - 
 *
 * @return int - 0: no error; 1: error occured
 * 
 * @todo reset menu graphics after printing buffer
 */
int printdispwin(char* buff);

/**
 * @brief clears the dispwin
 *
 * 
 * @return int - see return value for ncurses wclear()
 * 
 */
int resetdispwin();

/**
 * @brief maps the given function to the specified key. 
 *
 * Previously defined functions mapped for the given key will be overridden.
 * This function should rarely be used. If you are trying to map a given
 * function to a command then use addcmdbarevent(). NOTE: Some keys cannot be
 * overriden.  Below is a list of these keys: 
 *
 * * ESC_KEY (27)
 * * ENTER_KEY (10)
 * * KEY_F(1-12) (??)
 *
 * @param key - the key to map. The key value must be between 0 and NEVENTS (see
 * keyboard.h)
 * @param func - the function to map to. See keyevent_func for details.
 *
 * @return int - error code: 0 - no error; 1 - error occured
 * 
 * @see removekeyevent()
 */
int addkeyevent(int key, keyevent_func func);

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
 * @return int - see addkeyevent() return cases  
 * 
 * @see addkeyevent
 */
int removekeyevent(int key);

/**
 * @brief maps the given function to the window for a command event.
 * 
 * The given function will be called if the current window in "focus" is the one
 * held by the keyboard controller and the ENTER_KEY is pressed.
 *
 * @param win - the ncurses window to map the function to
 * @param func - the function to add.
 *
 * @return int - 0 if no error, 1 if error occurs
 *
 * @see cmdevent_func
 * 
 */
int addcmdevent(WINDOW* win, cmdevent_func func);

/**
 * @brief removes the cmdevent for a given WINDOW
 * 
 * "removes" the mapped cmdevent_func to the given window by replacing the
 * function with a "do nothing" function.
 *
 * @param win - an ncurses window
 *
 * @return void
 * 
 */
void removecmdevent(WINDOW* win);

/**
 * @brief adds a given function pointer to the set of functions that can be
 * called via the command bar.
 *
 * Commands via the command bar are given in the form: 
 * <code><funcname>([args])</code>
 *
 * args will be passed to the mapped function as a c-string.
 * 
 * @param func -  the function pointer (see cmdbarevent_func)
 * @param funcalias - the name (or alias) of the function. This is what will be
 * called by the user in the command bar
 *
 * @return int - 0: no error; 1: error occured
 * @see removecmdbarfunc()
 * @see cmdbarevent_func
 *
 * @todo: rewrite with hasmap implementation
 * 
 */
int addcmdbarfunc(cmdbarevent_func func, char* funcalias);

/**
 * @brief removes the function pointer with the given function alias from the
 * stack of functions to be called by the command bar.
 *
 * @param funcalias - the name (or alias) given for the function. If no function
 * by this name is found nothing happens
 *
 * @return void
 * @see addcmdbarfunc()
 * 
 * @todo: rewrite with hashmap implementation
 */
void removecmdbarfunc(char* funcalias);

/**
 * @brief fetches the cmdbarevent_func matching the given alias
 * 
 * if more than one function has the same alias then the first function that is
 * found is returned.
 *
 * @param alias - the alias of the function
 *
 * @return cmdbarevent_func - command bar function reference
 * 
 */
cmdbarevent_func getcmdbarfunc(char* alias);

#endif 
