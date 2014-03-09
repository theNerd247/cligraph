//old includes
#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "graph.h"
#include "stringmath.h"
#include "table.h"

//new includes
#include <ncurses.h>
#include "cligraph.h" 
#include "dbg.h"

#define ENTER_KEY 10 

int xsize = 0;
int ysize = 0;
double domain;
double range;

typedef struct tble_point
{
	int x;
	int y;
} TBLPOINT;

double getdy(FuncValues* vals)
{
	int i;
	Node* curr = vals->head;
	double mx = ((POINT*)curr->data)->y;
	double mn = mx;
	for (i = 0; i < vals->length; i++)
	{
		double nwmx = ((POINT*)curr->data)->y;
		if(nwmx > mx) mx = nwmx;
		if(nwmx < mn) mn = nwmx;
		curr = curr->next;
	}
	
	return mx-mn;
}

TBLPOINT* mapPoint(POINT* pnt)
{
	TBLPOINT* newpnt = (TBLPOINT*)malloc(sizeof(TBLPOINT));
	VALIDPNTR(newpnt, NULL);

	double xcoef = xsize/domain;
	double ycoef = ysize/range;

	newpnt->x = (int)(pnt->x*xcoef);
	newpnt->y = (int)(pnt->y*ycoef);

	return newpnt;
}

void spacetbl(struct table* tbl)
{
	int i,j;
	for (i = 0; i < tbl->x_size; i++)
	{
		for (j = 0; j < tbl->y_size; j++)
		{
			setcell(tbl,' ',i,j);
		}
	}
}

struct table* makegraph(FuncValues* values)
{
	VALIDPNTR(values, NULL);

	range = getdy(values);
	domain = ((POINT*)llgetvalue(values, values->length-1))->x;

	struct table* newtble = inittble(xsize,ysize);
	VALIDPNTR(newtble, NULL);	
	spacetbl(newtble);

	int i;
	for (i = 0; i < values->length; i++)
	{
		POINT* pnt = (POINT*)llgetvalue(values,i);
		TBLPOINT* tpnt = mapPoint(pnt);
//		printf("X: %i Y: %i\n", tpnt->x,tpnt->y);
		setcell(newtble,'#',tpnt->x,tpnt->y);	
	}
	
	return newtble;
}

void graphfunc(char* buff)
{
	if(buff == '\0')
		return;

	xsize = COLS-4;
	ysize = LINES-10; 
	FuncValues* values = getfuncvalues(buff,.01,1,.01);

/*
 * 	char buffer[values->length*18];
 * 	int ind = 0;
 * 
 * 	POINT* point = NULL;
 * 	size_t pvalue(void* data)
 * 	{
 * 		point = (POINT*)data;
 * 		sprintf(buffer+ind," x=%5.0f y=%5.0f\n",point->x,point->y);
 * 		ind+=17;
 * 		return 0;
 * 	}
 * 
 * 	llapply(values,pvalue);
 * 	printdispwin(buffer);
 * 	 */

	struct table* graph = makegraph(values);
	pgraph(graph);
}

void* startgraph(void* null)
{
	addcmdbarfunc(graphfunc,"graph");
	return NULL;
}
