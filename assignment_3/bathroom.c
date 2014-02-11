// Ted Meyer
// Bathroom.c
// License: GPL v2

#include "bathroom.h"
#include <stdio.h>
#include <time.h>

//enum gender = 
//{
//	MALE,
//	FEMALE,
//	NONE
//};


gender occupying_sex;
int occupancy_count;

//statistics

int highest_men;
int highest_women;
int total_men;
int total_women;





void initialize()
{
	occupying_sex = NONE;
	occupancy_count = 0;
	int highest_concurrent = 0;
	gender popular_user = NONE;
	printf("initialized\n");
}

void finalize()
{
	printf("at maximum, there were:\n\tmen:   %i\n\twomen: %i\nand a total or %i men and %i women entered the bathroom\n",
		   highest_men, highest_women, total_men, total_women);
}



void enter(gender g)
{
	while(occupying_sex != NONE  &&  occupying_sex != g)
	{
		//wait
	}
	occupying_sex = g;
	occupancy_count++;

	
	if (g == MALE)
	{
		total_men++;
		if(occupancy_count > highest_men)
		{
			highest_men = occupancy_count;
		}
	}
	else if (g == FEMALE)
	{
		total_women++;
		if(occupancy_count > highest_women)
		{
			highest_women = occupancy_count;
		}
	}
	else
	{
		printf("a genderless person entered!\n");
	}



	return;
}


void leave()
{
	--occupancy_count;
	if (occupancy_count <= 0)
	{
		occupancy_count = 0;
		occupying_sex = NONE;
	}
}
