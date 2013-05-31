/*
 * strlib.c is part of cligraph.
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
 * strlib.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: definition of strlib functions 
 */

//from c std libs
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

//from project libs
#include "strlib.h"

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

char* strrpl(char* expr,const int strt, const int end, char* needle)
{
	int len = strlen(expr)+strlen(needle)-end+strt;
	char* newexpr = (char*)malloc(sizeof(char)*len);
	newexpr = strsub(expr,0,strt-1);
	char* tok1 = strsub(expr,end+1,strlen(expr)-1);
	
	strcat(newexpr,needle);
	strcat(newexpr,tok1);

	return newexpr;
}

double strtonum(char* expr)
{
	int pwr = 0;
	int i = strlen(expr)-1;
	int value = 0;
	for (; i >= 0; i--)
	{
		value = (*(expr+i)-48)*(pow(10,pwr))+value;
		pwr++;
	}
	
	return (double)value;
}
