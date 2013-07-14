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

int main(int argc, char const *argv[])
{
/*
 * 	char tststr[100];	
 * 	printf("expression:\n");
 * 	scanf("%s",tststr);
 * 
 * 	printf("min:\n");
 * 	double min[1]; 
 * 	scanf("%lf",min);
 * 
 * 	printf("max:\n");
 * 	double max[1]; 
 * 	scanf("%lf",max);
 * 
 * 	printf("stp:\n");
 * 	double stp[1]; 
 * 	scanf("%lf",stp);
 * 	 */
	
	char* tststr = "2x^2+3x+4";
	double min = 0;
	double max = 100;
	double stp = 1;

	char* tmp = expndexpr(tststr);
	FuncValues* values = getfuncvalues(tmp,min,max,stp);
	
	int i;
	for (i = 0; i < values->length; i++)
	{
		POINT* value = (POINT*)llgetvalue(values,i);
		printf("%i, %i\n", (int)value->x,(int)value->y);
	}
	return 0;
}
