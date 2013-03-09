/*
 * table.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: defines the table and functions thereof
 *
 */

#ifndef __TABLE
#define __TABLE

//typedef of void pointer to data to be held in the cell
typedef void* cell;

//structure that holds all the data cells of a table
struct table
{
	int x_size;
	int y_size;
	cell *cells;
};

/*
 * FUNCTION: init_table
 * 
 * PARAMETERS: int x_size, int y_size
 *
 * RETURNS: struct table* - a blank table
 * 
 * DESCRIPTION: creates and initiates a table pointer that has all data cell values to 0
 * 
 * ERROR CODES: NULL pointer (if any error occurs)
 */
struct table* init_table(int x_size, int y_size);

/*
 * FUNCTION: set_cell
 * 
 * PARAMETERS: table* tble, cell data_value, int x, int y
 *
 * RETURNS: int - error code 
 * 
 * DESCRIPTION: sets the data value at the given index
 *
 * ERROR CODES: -1 - no error; 0 - table pointer is invalid; 1 - cell is an invalid pointer; 2 - x and/or y is an
 * invalid index
 */
int set_cell(struct table* tble, cell data_cell, int x, int y);

/*
 * FUNCTION: get_cell
 * 
 * PARAMETERS: struct table* tble, int x, int y
 *
 * RETURNS: cell - a void pointer (see the typedef above)
 * 
 * DESCRIPTION: returns a cell at the given index
 *
 * ERROR CODES: NULL - null pointer (for any errors)
 */
cell get_cell(struct table* tble, int x, int y);

#endif 
