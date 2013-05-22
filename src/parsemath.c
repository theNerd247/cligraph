/*
 * parsemath.c is part of cligraph.
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
 * parsemath.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: code file for parsemath.h
 *
 */

#include "parsemath.h"
#include "string.h"
#include "stdlib.h"

int eval(char* expr, int value);

char* strins(char* dest, const char* source, const int index)
{
	int len = strlen(source)+strlen(dest)+1;

	char* newstr = (char*)malloc(sizeof(char)*len);

	strncpy(newstr, dest, index);
	strcpy(newstr+index, source);
	strcpy(newstr+index+strlen(source), dest+index);

	newstr[len-1] = '\0';

	return newstr;
}

char* strsub(char* expr, const int start, const int end)
{
	char* newstr = (char*)malloc(sizeof(char)*(end-start+2));

	int i;
	
	for (i = 0; i < end-start+1; i++)
		newstr[i] = expr[i+start]; 

	newstr[strlen(newstr)] = '\0';

	return newstr;
}

char* expndcoef(char* expr)
{
	if (expr == NULL || *expr == '\0')
		return expr;

	//find indep var	
	char* tok1;
	char* indeppos = (char*)memchr(expr, indep_var, strlen(expr));

	if(indeppos == NULL)
		return expr;

	//check for coef where indep var is found and then add the rest. 
	tok1 = strsub(expr,0,indeppos-expr);
	if(strpbrk(indeppos-1,"0123456789") != NULL)
		tok1 = strins(tok1, "*", indeppos-expr);

	char* tok2 = expndcoef(indeppos+1);
	strcat(tok1,tok2);

	return tok1;
}

char* expndexpr(char* expr)
{
	char* newexpr;
	//--Outline------------------------------
	/*
 	 * 1. replace number-letter pairs with multiplication
 	 * 2. put parenthesis around all operations to declare order
 	 * 3. 
 	 */
	//--END Outline---------------------------
	
	newexpr = expndcoef(expr);

	return newexpr;
}
