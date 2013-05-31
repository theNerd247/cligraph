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

//from c std libraries
#include "parsemath.h"
#include "string.h"
#include "stdlib.h"

//from project libraries
#include "strlib.h"

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

char* inspare(char* expr)
{	
	//insert at begining and end of string	
	char* newexpr;
	newexpr = strins(expr,"(",0);
	newexpr =	strins(newexpr,")",strlen(newexpr));

	//insert around pre-existing parenthesis
	int para_flag = ((char*)memchr(newexpr+1,'(',strlen(newexpr)))-newexpr;	
	int epara_flag = 0;

	while(para_flag >= 0 )
	{
		newexpr = strins(newexpr,"(",para_flag);

		epara_flag = ((char*)memchr(newexpr+para_flag,')',strlen(newexpr+para_flag+1)))-newexpr;

		if(epara_flag >= 0)
			newexpr = strins(newexpr,")",epara_flag);

		para_flag = ((char*)memchr(newexpr+epara_flag,'(',strlen(newexpr)))-newexpr;	
	}

	//insert before and after '+' and/or '-'
	int p_flag = strpbrk(newexpr,"+-")-newexpr;
	
	while(p_flag >= 0)
	{
		newexpr = strins(newexpr,")",p_flag);
		newexpr = strins(newexpr,"(",p_flag+2);

		p_flag = strpbrk(newexpr+p_flag+2,"+-")-newexpr;
	}

	return newexpr;
}

char* expndexpr(char* expr)
{
	char* newexpr;

	newexpr = expndcoef(expr);
	newexpr = inspare(newexpr);

	return newexpr;
}