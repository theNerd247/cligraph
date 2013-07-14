/*
 * stringmath.c is part of cligraph.
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
 * stringmath.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: source file for stringmath.h
 */

//from std libs
#include <stdlib.h>
#include <string.h>
#include <math.h>

//from project libs
#include "strlib.h"
#include "parsemath.h"
#include "stringmath.h"

char chknum(char* expr)
{
	int i;
	int len = strlen(expr);
	char val = 1;	
	for (i = 0; i < len; i++)
	{
		char tmp = *(expr+i);
		if(tmp == '.')
			continue;	
		if(tmp < 48 || tmp > 57) 	
		{
			val = 0;
			break;
		}
	}
	return val;
}

double strtonum(char* expr)
{

	double pwr = 0.0;
	double value = 0.0;
	int i = strlen(expr)-1;

	char* decindex = strpbrk(expr,".");
	if(decindex != NULL)
		pwr = (double)(decindex-(expr+i));

	for (; i >= 0; i--)
	{
		if(*(expr+i) == '.')
			continue;
		value = (*(expr+i)-48)*(pow(10,pwr))+value;
		pwr++;
	}
	
	return value;
}

double getvalue(char* expr, double value)
{
	//check if expression is numbers only
	if(chknum(expr) == 1)
		return strtonum(expr);
	else if(*expr == indep_var && strlen(expr) == 1)
		return value;

	//if not then check for operations
	char* op = strpbrk(expr,"+-");
	if(op == NULL)
		op = strpbrk(expr,"*/");
	if(op == NULL)
		op = strpbrk(expr,"^");
	//avoid case where expr == "(x)"
 	if(op == NULL)
		return getvalue(parntrim(expr),value);
	
	double val;
	double lftval, rhtval;
	if(op != NULL)
	{
		char* lftexpr = strsub(expr,0,op-expr-1);
		char* rhtexpr = strsub(expr,op-expr+1,strlen(expr)-1);

		//rm () on ends of str
		if(strpbrk(lftexpr,"()") != NULL)
			lftexpr = parntrim(lftexpr);

		if(strpbrk(rhtexpr,"()") != NULL)
			rhtexpr = parntrim(rhtexpr);

		lftval = getvalue(lftexpr,value);
		rhtval = getvalue(rhtexpr,value);

		switch(*op)
		{
			case '+':
				val = lftval+rhtval;
				break;
			case '-':
				val = lftval-rhtval;
				break;
			case '*':
				val = lftval*rhtval;
				break;
			case '/':
				val = lftval/rhtval;
				break;
			case '^':
				val = pow(lftval,rhtval);
				break;
			default:
				val = lftval;
				break;	
		}
	}

	return val;
}

//warning: untested
FuncValues* getfuncvalues(char* expr, double strt, double end, const double step)
{
	int N = (int)((end-strt)/step);
	FuncValues* values = llnew();

	int i;
	double val;
	for (i = 0; i <= N; i++)
	{
		double ival = strt+(step*i);
		val = getvalue(expr,ival);
		POINT* newpoint = newPOINT();
		newpoint->x = ival;
		newpoint->y = val;
		llappend(values, newpoint);
	}

	return values;
}
