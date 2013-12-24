#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "dbg.h"
#include "cligraph.h"

int printmsg()
{
	debug("printmsg");
	return 0;
}

void* starttst(void* null)
{
	addkeyevent(10,printmsg);	
	return NULL;
}
