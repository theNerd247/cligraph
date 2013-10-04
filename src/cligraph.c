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

int main(int argc, char const *argv[])
{
	//--INIT------------------------------
	initscr();
	//--END INIT---------------------------

	WIN* win1 = winnew(100,9,100,100);

	//draw some stuff
	box(win1->window,'|','-');
	wprintw(win1->window,"HELLO WORLD!");
	wrefresh(win1->window);

	windel(win1);
	
	getch(); //pause
	endwin();
	return 0;
}
