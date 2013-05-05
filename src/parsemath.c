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
 */

#include "parsemath.h"
#include "string.h"

int eval(char* expr, int value)
{
}

//insert source into destination at given index
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

char* replaceops(char* expr, const char* regexpr, const char op)
{
	char* newexpr;
	int len = strlen(expr);

	//possibly replace with switch statement
	if(op = '*')
	{
		//find indep var	
		char* indeppos = (char*)memchr(expr, indep_var, len);
		
		//check for coe
		if(indeppos != expr)
		{
			if(strpbrk(indeppos-1,"0123456789") != NULL)
			{
				newexpr = strins(expr, "*", indeppos-1);
				printf("%s\n", newexpr);
			}
		}
	}
}

char* expndexpr(char* expr)
{
	//--Outline------------------------------
	/*
 	 * 1. replace number-letter pairs with multiplication
 	 * 2. put parenthesis around all operations to declare order
 	 * 3. 
 	 */
	//--END Outline---------------------------
	
	//--Replace ops------------------------------
		
	//--END Replace ops---------------------------
}

