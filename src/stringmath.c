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
#include <string.h>
#include <strlib.h>
#include <math.h>

//from project libs
#include "table.h"
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
		if(tmp < 48 || tmp > 57) 	
		{
			val = 0;
			break;
		}
	}
	return val;
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
	
	double val;
	int lftval, rhtval;
	if(op != NULL)
	{
		char* lftexpr = strsub(expr,0,op-expr-1);
		char* rhtexpr = strsub(expr,op-expr+1,strlen(expr)-1);

		//rm () on ends of str
		if(strpbrk(lftexpr,"()") != NULL)
		{
			int pindex = lftexpr-mtchpar(lftexpr+strlen(lftexpr)-1,-1);
			lftexpr = strsub(lftexpr,pindex+1,strlen(lftexpr)-2);
		}

		if(strpbrk(rhtexpr,"()") != NULL)
		{
			int pindex = mtchpar(rhtexpr,1)-rhtexpr;
			rhtexpr = strsub(rhtexpr,1,pindex-1);
		}

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
				val = (int)lftval/rhtval;
				break;
			case '^':
				val = (int)pow(lftval,rhtval);
				break;
			default:
				val = lftval;
				break;	
		}
	}

	return val;
}

//warning: untested
struct table* getvalues(char* expr, double strt, double end, const double step)
{
	int N = (int)((end-strt)/step);
	struct table* values = init_table(N,1);

	int i;

	double val;
	for (i = 0; i < N; i++)
	{
		int ival = strt+step*i;
//		char* exprn = insindpvar(expr,ival);
		val = getvalue(expr,ival);
		set_cell(values,ival,i,0);
		set_cell(values,val,i,1);	
	}

	return values;
}
