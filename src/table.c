This file name is part of sofware name.

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

/*
 * table.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: c file for table.h
 */

#include "table.h"
#include "stdlib.h"
#include "stdio.h"

struct table* init_table(int x_size, int y_size)
{
	struct table* newtbl = (struct table*)(malloc(sizeof(struct table)));

	if (newtbl == NULL) 
		return NULL; 

	newtbl->cells = (cell*)(malloc(sizeof(x_size*y_size)));
	newtbl->x_size = x_size;
	newtbl->y_size = y_size;

	return newtbl;
}

int set_cell(struct table* tble, cell data_cell, int x, int y)
{
	if(tble == NULL) 
		return 0;

	if(data_cell == NULL) 
		return 1;

	if(x >= tble->x_size || y >= tble->y_size || x < 0 || y < 0)
		return 2; 


	tble->cells[x*(tble->y_size)+y] = data_cell;
//	printf("%i,%i - %i \n",x,y,*((int*)data_cell));
	return -1;
}

cell get_cell(struct table* tble, int x, int y)
{
	if(tble == NULL || x >= tble->x_size || y >= tble->y_size || x < 0 || y < 0) 
		return NULL;
	return tble->cells[x*(tble->y_size)+y];
}
