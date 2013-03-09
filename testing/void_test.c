#include "stdio.h"

int main(int argc, char const *argv[])
{
	int pint[1] = {23};
	void* pvoid = pint;

	printf("INT: %i - %i\n", *pint, *((int*)pvoid));
	return 0;
}
