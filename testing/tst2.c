#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

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

char* strsub(char* expr, const int start, const int end)
{
	char* newstr = (char*)malloc(sizeof(char)*(end-start+2));

	int i;
	
	for (i = 0; i < end-start+1; i++)
		newstr[i] = expr[i+start]; 

	newstr[strlen(newstr)] = '\0';

	return newstr;
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

int main(int argc, char const *argv[])
{
//	char* rtrn = popen("echo \"scale=6; 2*3\" | /usr/bin/bc -q","r");
//	int val = '+';
//	printf("%i\n",val );

	char* tst = "(2*3)-(4)";

	int chr = getvalue(tst);

	printf("%i\n",chr);
}
