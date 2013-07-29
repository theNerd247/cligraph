/*
 * graph.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: header file for functions handling the graph display functions
 *
 */

/*
 * graph.h is part of cligraph.
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

#include "table.h"
#include "stringmath.h"

#ifndef __GRAPH
#define __GRAPH

#ifndef PNTCHAR
#define PNTCHAR '#'
#endif

/*
 * STRUCT: tble_point
 * TYPEDEF: TBLPOINT 
 *
 * x and y coordinates of a point on the graph to display.
 *
 * NOTE: This is not the same as POINT found in stringmath.h
 *
 */
typedef struct tblpnt_st 
{
	int x;
	int y;
} TBLPOINT;

typedef struct graph_st
{
	LList* funcvals;
	Table* graphtbl;
	double xmax, ymax, xmin, ymin;
} Graph;

/*
 * FUNCTION: mappnts
 * 
 * PARAMETERS: LList* funcvalues, int xsize, int ysize
 *
 * RETURNS: LList* - newly created LList with data of nodes TBLPOINTs pointers
 * 
 * DESCRIPTION: maps the given graphs funcvals to a list of graph friendly
 * points (TBLPOINT*s). 
 *
 * NOTE: error codes: 0 - no error; >0 - something went wrong
 */
LList* mappnts(LList* vals, int xsize, int ysize);

/*
 * FUNCTION: mktable
 * 
 * PARAMETERS: LList* tpoints, int xsize, int ysize
 *
 * RETURNS: Table* - newly created table with plotted points
 * 
 * DESCRIPTION: creates the table of printable values from the given graph info
 *
 * NOTE: returns NULL if error occurs 
 */
Table* mkgraphtbl(LList* tpoints, int xsize, int ysize);

/*
 * FUNCTION: pgraph
 * 
 * PARAMETERS: Table* tble
 *
 * RETURNS: int - error codes: 0 - no error; 1 - tble is an invalid pointer; 2 - other
 * 
 * DESCRIPTION: prints the given table of values in a graph. 
 *
 * NOTES: this function prints the character representation of integers. To print the values of a
 * table please store the characters as integers in the table
 */
int pgraph(Table* tble);

/*
 * FUNCTION: mkaxis
 * 
 * PARAMETERS: Table* tbl, int x, int y
 *
 * RETURNS: int - error code
 * 
 * DESCRIPTION: inserts x and y axis at given locations of the given graph
 *
 * NOTE: error code: 0 - no error; >0 - something bad happened
 */
int mkaxis(Table* tbl, int x, int y);

#endif 
