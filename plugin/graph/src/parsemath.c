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
#include <str.h>


//from project libraries
#include "parsemath.h"

char* expndexpr(char* expr)
{
	
	//get proper newexpr size
	char* i;
	int c = 0;
	for (i = expr; *i != '\0' ; i++)
	{
		char t[2] = {*i,'\0'};
		if(strpbrk(t,"()+-") != NULL) c+=3;
		else if(strpbrk(t,"x")) c+=1;
	}

	char temp[strlen(expr)+c];
	char* newexpr = (char*)malloc(sizeof(char)*strlen(expr)+c);

	expndcoef(expr,temp);
	inspare(temp,newexpr);

	return newexpr;
}

char* expndcoef(char* expr, char* buff)
{
	char tok1[strlen(expr)+1], tok2[strlen(expr)+1];

	if (expr == NULL || *expr == '\0')
	{
		strcpy(buff,"\0");
		return "\0";
	}

	//find indep var	
	char* indeppos = (char*)memchr(expr, indep_var, strlen(expr));

	if(indeppos == NULL)
	{
		strcpy(buff,expr);
		return buff;
	}

	//check for coef where indep var is found and then add the rest. 
	strsub(expr,0,indeppos-expr,tok1);
	if(strpbrk(tok1+strlen(tok1)-2,"0123456789") != NULL)
	{
		*(tok1+(strlen(tok1)-1)) = '*';
		*(tok1+strlen(tok1)) = indep_var;
	}

	//get the rest of the expanded string
	expndcoef(indeppos+1,buff);
	strcpy(tok2,buff);

	if(*buff == '\0') *buff = ' ';
	strcpy(buff,tok1);

	if(*tok2 != '\0')
		strcat(buff,tok2);

	return buff;
}

char* inspare(char* expr, char* buff)
{	
	char temp[strlen(buff)+3];
	char newexpr[strlen(buff)+3];

	//insert at begining and end of string	
	strins(expr,"(",0,temp);
	strins(temp,")",strlen(temp),newexpr);

	//insert around pre-existing parenthesis
	char* para = strpbrk(newexpr+1,"(");
	int para_flag = para == NULL ? -1 : para-newexpr;

	int epara_flag = 0;

	while(para_flag >= 0)
	{
		strins(newexpr,"(",para_flag,temp);

		char* epara = ((char*)memchr(temp+para_flag,')',strlen(temp+para_flag+1)));
		epara_flag = epara == NULL ? -1 : epara-temp;

		if(epara_flag == -1) return NULL;

		strins(temp,")",epara_flag,newexpr);

		para = strpbrk(newexpr+para_flag+2,"(");
		para_flag = para == NULL ? -1 : para-newexpr;
	}

	//insert before and after '+' and/or '-'
	char* p_temp = strpbrk(newexpr,"+-");
	int p_flag = p_temp == NULL ? -1 : p_temp-newexpr;
	
	while(p_flag >= 0)
	{
		strins(newexpr,")",p_flag,temp);
		strins(temp,"(",p_flag+2,newexpr);

		p_temp = strpbrk(newexpr+p_flag+2,"+-");
		p_flag = p_temp == NULL ? -1 : p_temp-newexpr;
	}

	strcpy(buff,newexpr);
	
	return buff;
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

char* parntrim(char* expr, char* buff)
{
	char* temp = (char*)malloc(sizeof(char)*strlen(expr));

	int len = strlen(expr);
	char lind, rind;
	lind = 0;
	rind = strlen(expr)-1;

	if(*expr == '(')
		lind=1;
	if(*(expr+len-1) == ')')
		rind=len-2;

	strsub(expr,lind,rind,temp);
	strcpy(buff,temp);

	free(temp);

	return buff;
}
