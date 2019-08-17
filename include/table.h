/*
 * table.h is part of yadp.
 * 
 * yadp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * yadp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with yadp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file table.h 
 *
 * library for handling tables
 *
 * @author theNerd247 (nharvey@spsu.edu)
 * @copyright GNU Public License 2
 */

#ifndef TBLTYPE
/** 
 * @brief the type to use for storing data in the table
 *
 * define TBLTYPE __before__ including this header file to set the table type.
 * If no value is set char is used as the default type 
 */
#define TBLTYPE char 

typedef struct 
{
	int xsize; /**< x size of the table */
	int ysize; /**< y size of the table */
	TBLTYPE** cells; /**< the 2d array structure to store the table */
}table_t;
#endif

#ifndef __table_headerinc__

#ifndef __table_body
#define __table_body

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief inserts the given value into the table at x,y
 * 
 * @param tbl - the table to insert data into
 * @param val - the value to insert into the table
 * @param x - the x coordinate of the cell to insert into
 * @param y - the y coordinate of the cell to insert into
 *
 * @return table_t* - 0: the given table; NULL: error occured
 * 
 */
table_t* setcell(table_t* tbl, TBLTYPE val, unsigned int x, unsigned int y)
{
	if(tbl == NULL) 
		return NULL;
	else if(x >= tbl->xsize || y >= tbl->ysize || x < 0 || y < 0)
		return NULL; 

	tbl->cells[y][x] = val;
	return tbl;
}

/**
 * @brief initializes a new table of type TLBTYPE in dynamic memory
 *
 * @param x - the x size of the table
 * @param y - the y size of the table
 *
 * @return table_t* - 0: newly created table; NULL: error occured
 * 
 * @see TBLTYPE
 */
table_t* inittbl(TBLTYPE val, unsigned int x, unsigned int y)
{
	size_t i,j;
	table_t* newtbl = (table_t*)malloc(sizeof(table_t));

	if (newtbl == NULL) 
		goto error;

	//create row containers
	newtbl->cells = (TBLTYPE**)calloc(y,sizeof(TBLTYPE*));

	if(newtbl->cells == NULL)
		goto error;

	newtbl->xsize = x;
	newtbl->ysize = y;

	//create row cols
	for (i = 0; i < y; i++)
		newtbl->cells[i] = (TBLTYPE*)calloc(x,sizeof(TBLTYPE));

	//init table values
	for (i = 0; i < y; i++)
		for (j = 0; j < x; j++)
			setcell(newtbl,val,j,i);

	return newtbl;

	error:
		return NULL;
}

/**
 * @brief returns the value contained in the table at x,y
 *
 * @param tbl - table to fetch values from
 * @param x - x position in table (x >= 0)
 * @param y - y position in table (y >= 0)
 *
 * @return TBLTYPE - the value in the given cell. (0 is returned if an error occurs).
 * 
 */
TBLTYPE getcell(table_t* tbl, unsigned int x, unsigned int y)
{
	if(tbl == NULL)
		return 0;
	else if(x >= tbl->xsize || y >= tbl->ysize || x < 0 || y < 0) 
		return 0; 

	return tbl->cells[y][x];
}

/**
 * @brief frees the memory contained by a table
 * 
 * You must call this funtion if you call inittbl() or you will no be using
 * proper memory management.
 *
 * @param tbl - the table to delete.
 *
 * @return void
 * 
 */
void deltbl(table_t* tbl);

#endif //ifdef __table
#endif //ifdef __table_headerinc__
