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
#include "stdlib.h"
#include "llist.h"
#include "table.h"

int pgraph(Table* tble)
{
	//--ERROR CHECK------------------------------
	if(tble == NULL)
		return 1;
	//--END ERROR CHECK---------------------------

	//--Print Table------------------------------
	int i,j;
	int xsize = tble->xsize;
	int ysize = tble->ysize;
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

Table* mkgraphtbl(LList* tpoints, int xsize, int ysize)
{
	//init table with spaces
 	Table* newtbl = tblnew(xsize,ysize);
	VALIDPNTR(newtbl, NULL);

	int i,j;
	for (i = 0; i < newtbl->xsize; i++)
	{
		for (j = 0; j < newtbl->ysize; j++)
		{
			setcell(newtbl,' ',i,j);
		}
	}

	void plot(void* pnt)
	{
		TBLPOINT* tpnt = (TBLPOINT*)pnt;
		int x=tpnt->x;
		int y=tpnt->y;
		setcell(newtbl,PNTCHAR,x,y);
	}	

	llapply(tpoints,&plot);

	return newtbl;
}

//TODO: fix mapping algorithm
LList* mappnts(LList* vals, int xsize, int ysize)
{
	//get the max and min values of the x and y sets 
	//for proper zooming of the graph
	double ymx, xmx, ymn, xmn;

	ymx = ymn = ((POINT*)llgetvalue(vals,0))->y;
	xmx = xmn = ((POINT*)llgetvalue(vals,0))->x;

	void getxtrms(void* point)
	{
		POINT* pnt = (POINT*)point;
		double nymx = pnt->y;
		double nxmx = pnt->x;
		if(nymx > ymx) ymx = nymx;
		if(nymx < ymn) ymn = nymx;
		if(nxmx > xmx) xmx = nxmx;
		if(nxmx < xmn) xmn = nxmx;
	}	
	
	llapply(vals,&getxtrms);

	//get the zoom coeficient
	double xcoef = xsize/(xmx-xmn);	
	double ycoef = ysize/(ymx-ymn);	

	//plot the points on the table
	void* convert(void* point)
	{
		POINT* pnt = (POINT*)point;
		VALIDPNTR(pnt,NULL);
		TBLPOINT* tpnt = (TBLPOINT*)malloc(sizeof(TBLPOINT));

		tpnt->x = (int)(xcoef*(pnt->x-xmn));
		tpnt->y = (int)(ycoef*(pnt->y-ymn));	
		
		return (void*)tpnt;
	}	

	return llmap(vals, &convert);
}
