#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "cligraph.h"

int printmsg()
{
	void (*printdisp)(const char*);
	printdisp = getfuncref("tui","printdisp");
	printdisp("print from tst plugin\n");
	return 0;
}

void* starttst(void* null)
{
	int (*addkeyevent)(int,int(*func)(void));
	addkeyevent = getfuncref("tui","addkeyevent");
	addkeyevent(10,printmsg);
	return NULL;
}
