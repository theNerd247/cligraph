#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
		value = (*(expr+i)-48)*(pow(10,pwr))+value;
		pwr++;
	}
	
	return value;
}

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
		if(tmp < 48 || tmp > 57) 	
		{
			val = 0;
			break;
		}
	}
	return val;
}

double ch(double value)
{
	return value;
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

char* strsub(char* expr, const int start, const int end)
{
	char* newstr = (char*)malloc(sizeof(char)*(end-start+2));

	int i;
	
	for (i = 0; i < end-start+1; i++)
		newstr[i] = expr[i+start]; 

	newstr[strlen(newstr)] = '\0';

	return newstr;
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

int main(int argc, char const *argv[])
{
	char* expr = "(x)";
	printf("%s\n", parntrim(expr));
	expr = "x)";
	printf("%s\n", parntrim(expr));
	expr = "(x";
	printf("%s\n", parntrim(expr));
	return 0;
}
