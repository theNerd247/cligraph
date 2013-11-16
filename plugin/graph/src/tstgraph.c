//old includes
#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "graph.h"
#include "stringmath.h"
#include "table.h"

//new includes
#include "cligraph.h" 

#define ENTER_KEY 10 

int xsize = 100;
int ysize = 100;
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

	printf("domain: %lf range: %lf\n", domain, range);

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

/*
 * int main(int argc, char const *argv[])
 * {
 * 	char* expr = "x^0.5";
 * 	FuncValues* values = getfuncvalues(expr,0,50,1);
 * 	struct table* graph = makegraph(values);
 * 
 * //	double xc = xsize/domain;
 * //	double yc = ysize/range;
 * //	printf("%lf %lf\n",xc,yc);
 * 	
 * 
 * /*
 *  * 	int i;
 *  * 	for (i = 0; i < values->length; i++)
 *  * 	{
 *  * 		POINT* value = (POINT*)llgetvalue(values,i);
 *  * 		printf("%lf, %lf\n", value->x,value->y);
 *  * 	}
 *  * 	 /
 * 
 * 	pgraph(graph);	
 * 
 * 	return 0;
 * }
 * 
 *  */

/*
 * int graphfunc()
 * {
 * 	int (*getlastcmd)(char*);
 * 	getlastcmd = getfuncref("tui","getlastcmd");	
 * 	void (*printdisp)(const char*);
 * 	printdisp = getfuncref("tui","printdisp");
 * 
 * 	char buff[100];
 * 	getlastcmd(buff);
 * 	FuncValues* values = getfuncvalues(buff,0,50,1);
 * 	struct table* graph = makegraph(values);
 * 	printdisp((char*)graph->cells);
 * }
 * 
 *  */

/*
 * startgraph(void* null)
 * {
 * 	int (*addkeyevent)(int, int (*event_func)(void));
 * 	addkeyevent = getfuncref("tui","addkeyevent");
 * 	addkeyevent(ENTER_KEY,&graphfunc);
 * }
 * 
 *  */

int main(int argc, char const *argv[])
{
	char buff[100] = "2*(2x-2)/((x^2-2x)^(-2))";
	
	FuncValues* values = getfuncvalues(buff,0,50,1);
	struct table* graph = makegraph(values);
	pgraph(graph);

	return 0;
}
