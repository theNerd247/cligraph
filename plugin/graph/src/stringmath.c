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
#include <stdio.h>
#include <string.h>
#include <str.h>
#include <math.h>

//from project libs
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
		if(tmp == '-')
			continue; 
		if(tmp < 48 || tmp > 57) 	
		{
			val = 0;
			break;
		}
	}
	return val;
}

char chknumc(char expr)
{	
	char b[2] = {expr,'\0'};
	return chknum(b);
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
		if(*(expr+i) == '-')
			continue;
		value = (*(expr+i)-48)*(pow(10,pwr))+value;
		pwr++;
	}

	if(*expr == '-') value *= -1;	

	return value;
}

double getvalue(char* expr, double value)
{
	double val;
	double lftval, rhtval;
	char lftexpr[255], rhtexpr[255];
	char rplbuff[255], trimmed[strlen(expr)];
	parntrim(expr,trimmed);
	
	//check if expression is numbers only or is the indep_var
	if(chknum(expr) == 1 )
		return strtonum(expr);
	else if(chknum(trimmed))
		return strtonum(trimmed);
	else if(*expr == indep_var && strlen(expr) == 1)
		return value;
	
	//if not then check for operations
	char* op = strpbrk(expr,"^");
	if(op == NULL)
		op = strpbrk(expr,"*/");
	if(op == NULL)
	{
		op = strpbrk(expr,"+");
	}
	if(op == NULL)
	{
		op = strpbrk(expr,"-");
		if(op && *(op-1) == '(') op = NULL;
	}
	//avoid case where expr == "(x)"
	 	if(op == NULL)
		return getvalue(trimmed,value);
	
	if(op != NULL)
	{
		//get left and right expressions
	
		//get lftexpr 
		if(*(op-1) == ')')
		{
			int pind = mtchpar(op-1,-1)-expr;
			strsub(expr,pind,op-expr-1,lftexpr);
			char lt[strlen(lftexpr)];
			strcpy(lftexpr,parntrim(lftexpr,lt));
		} 	
		else if(chknumc(*(op-1)) == 1)
		{
			int i = 1;
			while(chknumc(*(op-i))) i++;
			i--;
			strsub(expr,op-expr-i,op-expr-1,lftexpr);
		}
		else
		{
			lftexpr[0] = 'x';
			lftexpr[1] = '\0';
		}
	
		//get rhtexpr
		if(*(op+1) == '(')
		{
			int pind = mtchpar(op+1,1)-expr;
			strsub(expr,op-expr+1,pind,rhtexpr);
			char rt[strlen(rhtexpr)];
			strcpy(rhtexpr,parntrim(rhtexpr,rt));
		} 	
		else if(chknumc(*(op+1)) == 1)
		{
			int i = 1;
			while(chknumc(*(op+i))) i++;
			i--;
			strsub(expr,op-expr+1,op-expr+i,rhtexpr);
		}
		else
		{
			rhtexpr[0] = 'x';
			rhtexpr[1] = '\0';
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
	
	char vl[12];
	sprintf(vl,"%lf",val);
	
	int lind = op-expr-strlen(lftexpr);
	int rind = op-expr+strlen(rhtexpr);
	
	if(*(op-1) == ')')
		lind-=1;
	if(*(op+1) == '(')
		rind+=1;
	
	strrpl(expr,vl,lind,rind,rplbuff);
	
	return getvalue(rplbuff, value);
}

//warning: untested
FuncValues* getfuncvalues(char* expr, double strt, double end, const double step)
{
	char* f = expndexpr(expr);
	int N = (int)((end-strt)/step);
	FuncValues* values = llnew();

	int i;
	double val;
	for (i = 0; i <= N; i++)
	{
		double ival = strt+(step*i);
		val = getvalue(f,ival);
		POINT* newpoint = newPOINT();
		newpoint->x = ival;
		newpoint->y = val;
		llappend(values, newpoint);
	}

	return values;
}
