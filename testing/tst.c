#include "stdio.h"
#include "parsemath.h"

int main(int argc, char const *argv[])
{
	char* expr = "2x+3x^3x-(x-3x)";	
	char* new = expndcoef(expr);

	printf("%s",new);
	return 0;
}
