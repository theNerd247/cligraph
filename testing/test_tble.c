/*
 * test_tble.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: testing file for table.c
 */
#include "table.h" 
#include "stdio.h"

//--TODO------------------------------
/*
 * Fix bug of passing value through set_cell(...) function call
 */
//--END TODO---------------------------

int main(int argc, char const *argv[])
{
	//--Init Values------------------------------
	const short ln = 100;
	int nums[ln];
	
	int i,j;
	for (i = 0; i < ln; i++)
	{
		nums[i] = i;
	}

	cell cells = nums;

	//--END Init Values---------------------------
	

	//--Init Table------------------------------
	struct table* newtable = init_table(10,10);
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			int* celz = cells;
			printf("CELZ - %i \n",*celz+(10i+j));
			set_cell(newtable,cells+(10*i+j),i,j);
		}
	}
	//--END Init Table---------------------------

	//--Read Table Values------------------------------
	cell testValue = get_cell(newtable,3,2);

	if(testValue == NULL)
		printf("ERROR OCCURED AT LINE 41");

	int* rror = (int*)testValue;

	printf("INDEX: 3,2 - ECODE: %i \n",i,j,*rror);
	//--END Read Table Values---------------------------

	return 0;
}
