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

struct table* normalize(struct table* tble)
{
	//attempt one notes: try drawing a line between each point.
	
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
	for (i = 0; i < xsize; i++)
	{
		for (j = 0; j < ysize; j++)
		{
			int cell = get_cell(tble,i,j);
			printf("%c",cell);
		}
		printf("\n");
	}	
	//--END Print Table---------------------------

	return 0;
}
