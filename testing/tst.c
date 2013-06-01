#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

char indep_var = 'x';

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

//inserts parenthesis to order the execution of the mathematical statement
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

char* strrep(char* expr,const int strt, const int end, char* needle)
{
	int len = strlen(expr)+strlen(needle)-end+strt;
	char* newexpr = (char*)malloc(sizeof(char)*len);
	newexpr = strsub(expr,0,strt-1);
	char* tok1 = strsub(expr,end+1,strlen(expr)-1);
	
	strcat(newexpr,needle);
	strcat(newexpr,tok1);

	return newexpr;
}

char* insindpvar(char* expr, char* value)
{
	//replace indepvar with value
	int iindx = ((char*)memchr(expr,indep_var,strlen(expr)))-expr;

	char* tmp;
	while(iindx >= 0)
	{
		tmp = strrep(expr,iindx,iindx,value);	
		iindx = ((char*)memchr(tmp,indep_var,strlen(tmp)))-tmp;
	}

	return tmp;
}

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

int getvalue(char* expr)
{
	//check if expression is numbers only
	if(chknum(expr) == 1)
		return strtonum(expr);

	//if not then check for operations
	char* op = strpbrk(expr,"+-");
	if(op == NULL)
		op = strpbrk(expr,"*/");
	
	int val, lftval, rhtval;
	if(op != NULL)
	{
		char* lftexpr = strsub(mtchpar(op-1,-1),0,op-expr-1);
		char* rhtexpr = strsub(op+1,0,mtchpar(op+1,1)-op+1);

		if(strpbrk(lftexpr,"()") != NULL)
			lftexpr = strsub(lftexpr,1,strlen(lftexpr)-2);	
		if(strpbrk(rhtexpr,"()") != NULL)
			rhtexpr = strsub(rhtexpr,1,strlen(rhtexpr)-2);

		lftval = getvalue(lftexpr);
		rhtval = getvalue(rhtexpr);

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
			default:
				val = lftval;
				break;	
		}
	}

	return val;
}

int strtonum(char* expr)
{
	int pwr = 0;
	int i = strlen(expr)-1;
	int value = 0;
	for (; i >= 0; i--)
	{
		value = (*(expr+i)-48)*(pow(10,pwr))+value;
		pwr++;
	}
	
	return value;
}

int main(int argc, char const *argv[])
{
	char tststr[] = "2x+3";

	char* tmp = expndcoef(tststr);
	tmp = inspare(tmp);
  tmp = insindpvar(tmp,"45");
	int val = getvalue(tmp);
	printf("%i\n", val);

	return 0;
}
