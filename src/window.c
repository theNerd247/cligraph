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
 * EMAIL: noah.harvey247 @gmail.com
 *
 * DESCRIPTION: definition file for prototypes in window.h
 */

#include "window.h"
#include <ncurses.h>
#include "stdlib.h"

WIN* winnew(size_t xsize, size_t ysize, size_t xpos, size_t ypos)
{
	//sanity checks
	if(xsize > COLS || ysize > LINES || xpos > COLS || ypos > LINES) return NULL;	

	//init structs		
	WINDOW* newWINDOW;
		if(!(newWINDOW = newwin(xsize, ysize, xpos, ypos))) return NULL;

	char* cntnt;
		if(!(cntnt = (char*)malloc(sizeof(char)*xsize*ysize))) return NULL;

	WIN* newWIN;
		if(!( newWIN = (WIN*)malloc(sizeof(WIN)))) return NULL;	

	newWIN->content = cntnt;
	newWIN->window = newWINDOW;
	newWIN->display = 0;

	return newWIN;
}

char windel(WIN* win)
{
	//sanity checks
	if(!win) return 0;

	//remove window from display
	if(win->display > 0)
	{
		if(wclear(win->window) == ERR) return 1;			
		if(wrefresh(win->window) == ERR) return 1;
	}

	//free that memory
	free(win->content);
	if(delwin(win->window) == ERR) return 1;
	
	return 0;
}
