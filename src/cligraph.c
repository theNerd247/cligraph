/*
 * cligraph.c is part of cligraph.
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

/*
 * cligraph.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: main source file for cligraph
 */

#include <stdio.h>

#include "stringmath.h"
#include "parsemath.h"
#include "graph.h"

int main(int argc, char const *argv[])
{		
	char* tstexpr = "(x-10)^3";

	struct bounds_st bnds;
	bnds.xmax = 1000;
	bnds.xmin = -500;
	LList* funcvls = getfuncvalues(tstexpr,bnds.xmin,bnds.xmax,1);

/*
 * 	void prnt(void* data)
 * 	{
 * 		POINT* pnt = (POINT*)data;
 * 		printf("%lf %lf\n",pnt->x,pnt->y);
 * 	}
 * 
 * 	llapply(funcvls,&prnt);
 * 	 */

	LList* tblvals = mappnts(funcvls,108,60); 
	Table* tbl = mkgraphtbl(tblvals,108,60);
	pgraph(tbl);	
	
	return 0;
} 
