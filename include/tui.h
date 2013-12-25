/**
 * tui.h is part of cligraph.
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
 * @file tui.h 
 *
 * @brief the functions that controll the tui core.
 *
 * These functions are not plugin safe unless otherwise specified.
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

/**
 * @internal 
 * -----------------TUI LAYOUT NOTES------------------------
 * The physical screen shall have the following layout: 
 *
 * --|-----|-- --> menu/toolbar section
 * --|-----|-- 
 *             --> output/interaction(for graph) section
 *         
 * ----------- --> input/command section
 * ----------- 
 * 
 * There shall be 6 "buttons" in the toolbar to switch "screens": 
 * 1. Math - performing mathmatical computations
 * 2. Functions - defining functions and relations
 * 3. Tables - a table editor
 * 4. Graphs - graphing ANYTHING
 * 5. Settings/Menu - contains menu for editing settings for plugins
 * 6. Reserved for Plugins - window where plugins can display custom tui
 *
 * Each screen will be accessed by a menu located in the menu section of the
 * screen. The menus 1-6 are to remain static throughout any given screen. No
 * plugin will have access to these menu items - except screen 6 which only
 * allows plugins to submit a menu object for addition as a sub-menu.
 *
 * Notes on Screen 6 Menu
 * Screen 6 shall display only one custom screen at any given time.
 * While only one screen is to be displayed a plugin is not limited to the
 * number of screens it has access to. The menu for screen 6 will be organized
 * into a single sub-menu for each plugin. The number of menu items per plugin
 * sub-menu is left up to the plugin, however each menu item must correspond to
 * a screen controlled by the plugin.
 *
 *
 */

#include <ncurses.h>

#ifndef __TUI
#define __TUI

/** the min ASCII value for printable characters*/
#define PRINTKEY_RANGE_MIN 32
/** the max ASCII value for printable characters */
#define PRINTKEY_RANGE_MAX 126

/** how fast to update the controll loops so we don't use up all our memory */
#define DOUPDATE_INTERVAL 10000000

/**
 * 
 * @brief adds an item to the given menu (0-5) for other plugins
 *
 * PARAMETERS: size_t num, ITEM* item
 *
 * @return int - error code: 0 - no error, 1 - something went wrong
 * 
 */
//int addmenuitem(size_t num, ITEM* item); don't make this callable yet

/**
 * @brief tells the tui controller that the calling thread is ready to move on.
 *
 * The thread that controls the tui manager will sometimes need to wait till
 * crucial data has been setup and so it waits. Calling this function will tell
 * the tui thread that it's ok to proceed if it's waiting on a child thread to
 * reach a checkpoint. Call this function at the end of a thread init function.
 *
 * 
 * @return void
 * 
 */
void tui_ready();

/**
 * 
 * @brief starts running the tui as a thread-safe system
 *
 * Called as the first "plugin" for cligraph. This sets up the core tui structs
 * and managers.
 *
 * @param null - absolutely nothing
 *
 * @return void* - this will be NULL no matter what. errno is set if error
 * occures
 */
void* starttui(void* null);

/**
 * 
 * @brief gracefully stops running the tui 
 *
 * This function: frees all memory used by the tui manager, kills all running
 * subthreads. If an error occurs during the stop process this function will
 * perform a not-so-gracefull shutdown.
 *
 * @return void - 
 * 
 */
void stoptui();

/**
 * @brief attempts to execute the currently inputed command.
 *
 * This is the function that is called when the ENTER_KEY is pressed (notice it
 * follows the event_func_type typedef). Never call this function directly as it
 * is already loaded at startup and cannot be removed or overridden. If you wish
 * to have a function called when the enter key is pressed see addcmdevent().
 *
 * @param key - the ENTER_KEY
 *
 * @return int - allways 0
 * 
 */
int sendcmd(int key);

#endif 
