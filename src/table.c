/* This file name is part of sofware name.

sofware name is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofware name is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofware name.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * table.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: c file for table.h
 */

//from std libs
#include <stdlib.h>
#include <stdio.h>

//from prjct libs
#include "table.h"

Table* tblnew(int xsize, int ysize)
{
	Table* newtbl = (Table*)(malloc(sizeof(Table)));

	if (newtbl == NULL) 
		return NULL; 

	newtbl->cells = (int*)calloc(xsize*ysize,sizeof(int));

	if(newtbl->cells== NULL)
		return NULL; 

	newtbl->xsize = xsize;
	newtbl->ysize = ysize;

	return newtbl;
}

int getcell(Table* tble, int x, int y)
{
	if(tble == NULL)
		return 0;
	else if(x >= tble->xsize || y >= tble->ysize || x < 0 || y < 0) 
		return 1; 

	return tble->cells[x*(tble->ysize)+y];
}

int setcell(Table* tble, int cell_value, int x, int y)
{
	if(tble == NULL) 
		return 0;

	if(x >= tble->xsize || y >= tble->ysize || x < 0 || y < 0)
		return 1; 


	int index = x*(tble->ysize)+y;
	*(tble->cells+(index)) = cell_value;
	return -1;
}
