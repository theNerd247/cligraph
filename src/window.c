/*
 * window.c is part of cligraph.
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
 * window.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: declaration file for window functions
 */

#include "ncurses.h"
#include "llist.h"

//window structure
typedef struct win_st
{
	WINDOW* window; /* ncurses window struct */
	char* content; /* the text to show in the window */
	int x,y; /* position of the window */
} WIN;

/*
 * FUNCTION: drawwin
 * 
 * PARAMETERS: WIN* win
 *
 * RETURNS: int - error code
 * 
 * DESCRIPTION: draws the window on the screen
 *
 * NOTE: error codes: 0 - function exited correctly; 1 - something bad happened
 */
int drawwin(WIN* win);
