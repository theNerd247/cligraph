#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "dbg.h"
#include "cligraph.h"

void printmsg(char* cmd)
{
	printdispwin(cmd);
}

void* starttst(void* null)
{
	addcmdbarfunc(&printmsg,"print");
	return NULL;
}
