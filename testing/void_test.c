#include "stdio.h"

int main(int argc, char const *argv[])
{
	int pint[2] = {23,44};
	void* pvoid = pint;

	printf("INT: %i - %i\n", *pint, *((int*)pvoid)+1);
	return 0;
}
