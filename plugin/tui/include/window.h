/*
 * window.h is part of cligraph.
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
 * window.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: declaration file for window functions
 */

#include <ncurses.h>

#ifndef __WINDOW
#define __WINDOW

#define CELLX 5
#define CELLY 9

//window structure
typedef struct win_st
{
	WINDOW* window; //ncurses window struct
	char* content;  //the content of the window
	char display;   //is the window to be shown? (0 = false, 1 = yes, but the window has yet to be drawn, 2 = yes, and the window has been drawn)
	char border;    //do we want a border to exist?
} WIN;

/*
 * FUNCTION: winnew
 * 
 * DESCRIPTION: creates a new window
 *
 * PARAMETERS: size_t x_px, size_t y_px, size_t xpos, size_t ypos
 *
 * RETURNS: WIN* - newly created window
 * 
 */
WIN* winnew(size_t xsize, size_t ysize, size_t xpos, size_t ypos);

/*
 * FUNCTION: windel
 * 
 * DESCRIPTION: removes the window. If the window is being displayed it is removed from the screen.
 *
 * PARAMETERS: WIN* win
 *
 * RETURNS: char - error code (0 - no error, 1 - error removing from screen, 2 - error freeing
 * memory
 * 
 */
char windel(WIN* win);

/*
 * FUNCTION: windisplay
 * 
 * DESCRIPTION: displays the window on the screen
 *
 * PARAMETERS: WIN* win
 *
 * RETURNS: WIN* - pointer to win
 * 
 */
WIN* windisplay(WIN* win);

#endif 
