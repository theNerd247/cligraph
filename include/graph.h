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

#include "stdio.h"
#include "table.h"

#ifndef __GRAPH
#define __GRAPH

/*
 * FUNCTION: pgraph
 * 
 * PARAMETERS: table* tble
 *
 * RETURNS: int - error codes: -1 - no error; 0 - tble is an invalid pointer; 1 - other
 * 
 * DESCRIPTION: prints the given table of values in a graph
 *
 */
int pgraph(table* tble);

#endif 
