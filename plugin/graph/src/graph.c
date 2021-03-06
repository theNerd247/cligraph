/*
 * graph.c 
 *
 * AUTHOR: Noah Harey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: header file for displaying the graph
 */

/*
 * graph.c is part of cligraph.
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

#include "graph.h"
#include "stdio.h"

char axischr = '|';
char pntchr = '+';
char gridchr = '#';
char gridflg = 0;

struct table* makefancy(struct table* tble)
{
 /*----NOTES----
 	* Assume: - X = {0,1,2,...,n}
 	* 				- y exists for each x 
 	*/
}

int pgraph(struct table* tble)
{
	//--ERROR CHECK------------------------------
	if(tble == NULL)
		return 1;
	//--END ERROR CHECK---------------------------

	//--Print Table------------------------------
	int i,j;
	int xsize = tble->x_size;
	int ysize = tble->y_size;
	for (i = ysize-1; i >= 0; i--)
	{
		for (j = 0; j < xsize; j++)
		{
			int cell = getcell(tble,j,i);
			printf("%c",cell);
		}
		printf("\n");
	}	
	//--END Print Table---------------------------

	return 0;
}
