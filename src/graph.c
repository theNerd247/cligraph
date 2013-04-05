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

int pgraph(struct table* tble)
{
	//--ERROR CHECK------------------------------
	if(tble == NULL)
		return 1;
	//--END ERROR CHECK---------------------------

	int i,j;
	//--Print Table------------------------------
	for (i = 0; i < tble->y_size; i++)
	{
		for (j = 0; j < tble->x_size; j++)
		{
			int cell = *(tble->cells+(j*(tble->y_size)+i));
			printf("%c",cell);
		}
		printf("\n");
	}	
	//--END Print Table---------------------------

	return 0;
}
