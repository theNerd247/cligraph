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

//from  std libraries
#include <string.h>
#include <stdlib.h>

//from project libraries
#include "parsemath.h"
#include "strlib.h"
#include "stringmath.h"

char* expndcoef(char* expr)
{
 	if(!expr || *expr == '\0') return NULL;
	
	char* exprcpy = (char*)malloc(sizeof(char)*strlen(expr));
	strcpy(exprcpy, expr);
	
	char* indpos = strpbrk(exprcpy,"x");
	while(indpos)
	{
		int indi = indpos - exprcpy;
		if(indpos-1 >= exprcpy && chknumc(*(indpos-1)))
		{
			exprcpy = strins(exprcpy,"*",indi); 
			indi++;
		}
		indpos = strpbrk(exprcpy+indi+1,"x"); 
	}

	return exprcpy;
}

char* expndexpr(char* expr)
{
	char* newexpr;

	newexpr = expndcoef(expr);
	newexpr = inspare(newexpr);

	return newexpr;
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

char* mtchpar(char* expr, short pcnt)
{
	if(strpbrk(expr,"()") == NULL)
		return expr;

	char* pindex = pcnt > 0 ? expr+1 : expr-1;
	while(pcnt != 0)
	{
		if(*pindex == '(')
			pcnt+=1;
		else if(*pindex == ')')
			pcnt-=1;

		if(pcnt == 0 || *(pindex+1) == '\0')
			break;
		pindex = pcnt > 0 ? pindex+1 : pindex-1;
	}
	
	return pindex;
}

char* parntrim(char* expr)
{
	char* newexpr = expr;
	int len = strlen(newexpr);

	if(*newexpr == '(')
		newexpr = strsub(newexpr,1,len-1);

	len = strlen(newexpr);
	if(*(newexpr+len-1) == ')')
		newexpr = strsub(newexpr,0,len-2);

	return newexpr;
}

