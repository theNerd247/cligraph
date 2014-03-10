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
#include "parsemath.h"
#include "dbg.h"

#define ENTER_KEY 10 

int xsize = 0;
int ysize = 0;
double xmin,xmax;
double ymin,ymax;

typedef struct tble_point
{
	int x;
	int y;
} TBLPOINT;

void getmnmx(FuncValues* vals)
{
	POINT* pnt;
	size_t getmn(void* data)
	{
		pnt = (POINT*)data;
		if(pnt->y > ymax) ymax = pnt->y;
		if(pnt->y < ymin) ymin = pnt->y;

		return 0;
	}

	llapply(vals,getmn);
}

TBLPOINT* mapPoint(POINT* pnt)
{
	TBLPOINT* newpnt = (TBLPOINT*)malloc(sizeof(TBLPOINT));
	VALIDPNTR(newpnt, NULL);

	double xd = (pnt->x-xmin)*((double)xsize/xmax);
	double yd = (pnt->y-ymin)*((double)ysize/ymax);

	newpnt->x = (int)xd;
	newpnt->y = (int)yd;

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

	getmnmx(values);

	struct table* newtble = inittble(xsize,ysize);
	VALIDPNTR(newtble, NULL);	
	spacetbl(newtble);

//	char buffer[values->length*30+1];
//	int ind = 0;

	int i;
	for (i = 0; i < values->length; i++)
	{
		POINT* pnt = (POINT*)llgetvalue(values,i);
		TBLPOINT* tpnt = mapPoint(pnt);
//		sprintf(buffer+ind," x=%5.4f y=%5.4f xd=%i yd=%i\n",pnt->x,pnt->y,tpnt->x,tpnt->y);
//		ind+=30;
		setcell(newtble,'#',tpnt->x,tpnt->y);	
	}
	
//	resetdispwin();
//	printdispwin(buffer);
	return newtble;
}

void graphfunc(char* buff)
{
	if(buff == '\0')
		return;

	xsize = COLS-4;
	ysize = LINES-10; 
	xmin = .01;
	xmax = 1;
	FuncValues* values = getfuncvalues(buff,xmin,xmax,.01);

/*
 * 	char buffer[values->length*18];
 * 	int ind = 0;
 * 
 * 	POINT* point = NULL;
 * 	size_t pvalue(void* data)
 * 	{
 * 		point = (POINT*)data;
 * 		sprintf(buffer+ind," x=%5.4f y=%5.4f\n",point->x,point->y);
 * 		ind+=17;
 * 		return 0;
 * 	}
 * 
 * 	llapply(values,pvalue);
 * 	resetdispwin();
 * 	printdispwin(buffer);
 * 	 */

	struct table* graph = makegraph(values);
	resetdispwin();
	pgraph(graph);
}

//divides the given expr of arguments into their respective args
void getargs(char* expr, char** buff, int size)
{	
	size_t i;
	char* tmp = strtok(expr,",");
	for (i = 0; i < size; i++)
	{
		if(tmp)
			strcpy(buff[i],tmp);
		tmp = strtok(NULL,",");
	}	
}

void eval(char* expr)
{
	char buff1[255], buff2[255], buff3[255];
	char* buff[2] = {buff1,buff2};

	getargs(expr,buff,2);	
	double val = getvalue(expndexpr(buff[0]),strtonum(buff[1]));
	
	sprintf(buff3,"%s ;x=%s\n\t = %f",buff[0],buff[1],val);
	printdispwin(buff3);
}

void* startgraph(void* null)
{
	addcmdbarfunc(graphfunc,"graph");
	addcmdbarfunc(eval,"eval");
	return NULL;
}
