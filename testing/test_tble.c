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
		nums[i] = i;
	//--END Init Values---------------------------
	

	//--Init Table------------------------------
	struct table* newtable = init_table(10,10);

	if(newtable == NULL)
		return -1;
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			set_cell(newtable,nums[10*i+j],i,j);
		}
	}
	//--END Init Table---------------------------

	//--Read Table Values------------------------------
	int testValue = get_cell(newtable,3,2);

	printf("INDEX: 3,2 - ECODE: %i \n",testValue);

	//--END Read Table Values---------------------------

	return 0;
}
