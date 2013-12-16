#include <stdlib.h>
#include <stdio.h>

void* starttst(void*);

int main(int argc, char const *argv[])
{
	starttst(NULL);
	return 0;
}

void* wrong(void* null)
{
	printf("wrong function\n");
	return NULL;
}
