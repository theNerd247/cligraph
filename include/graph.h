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

#ifndef __GRAPH
#define __GRAPH

/*
 * FUNCTION: makefancy 
 * 
 * PARAMETERS: struct table* tble
 *
 * RETURNS: struct *table - normalized points
 * 
 * DESCRIPTION: takes the given points to graph and adds extra points to make curves look as curves
 */
struct table* makefancy(struct table* tble);

/*
 * FUNCTION: frmtgraph
 * 
 * PARAMETERS: struct table* tble
 *
 * RETURNS: struct table* - newly formatted table
 * 
 * DESCRIPTION: formats the given table to look pretty (inserts axis, grid, etc..)
 */
struct table* frmtgraph(struct table* tble);

/*
 * FUNCTION: pgraph
 * 
 * PARAMETERS: struct table* tble
 *
 * RETURNS: int - error codes: 0 - no error; 1 - tble is an invalid pointer; 2 - other
 * 
 * DESCRIPTION: prints the given table of values in a graph. 
 *
 * NOTES: this function prints the character representation of integers. To print the values of a
 * table please store the characters as integers in the table
 */
int pgraph(struct table* tble);

#endif 
