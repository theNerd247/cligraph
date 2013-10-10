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

/*
 * cligraph.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: main source file for cligraph
 */

#include <stdio.h>

#include <ncurses.h>
#include "window.h"
#include "string.h"


int main(int argc, char const *argv[])
{
	//--INIT------------------------------
	initscr();
	//--END INIT---------------------------

	WIN* win1 = winnew(20,9,10,10);
	if(!win1) return 0;

	//draw some stuff
	win1->display = 1;
	strcpy(win1->content,"HELLO WORLD");	

	windisplay(win1);
	doupdate();
	wgetch(win1->window); //pause

	win1->border = 0;
	win1->display = 1;
	strcpy(win1->content,"HELLO WORLD THIS IS MULTILINE TEXT");	

	windisplay(win1);
	doupdate();

	wgetch(win1->window); //pause
	//ending sequence
	if(windel(win1)) return 1;
	
	endwin();
	return 0;
}
