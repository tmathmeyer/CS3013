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


int men = 0;
int women = 0;

//statistics

int highest_men;
int highest_women;
int total_men;
int total_women;



int state_lock = 0;
gender lock_sex = MALE;

void lock(gender g)
{
	state_lock = 1;
	lock_sex = g;
}

void unlock()
{
	if (men == 0 && women == 0)
	{
		state_lock = 0;
	}
}



void initialize()
{
	printf("initialized\n");
}

void finalize()
{
	printf("at maximum, there were:\n\tmen:   %i\n\twomen: %i\nand a total or %i men and %i women entered the bathroom\n",
		   highest_men, highest_women, total_men, total_women);
}



void enter(gender g)
{
	if (g == MALE)
	{
		while(lock_sex==FEMALE && state_lock==1);
		lock(g);
		men++;
		total_men++;
		if(men > highest_men)
		{
			highest_men = men;
		}
	}
	else if (g == FEMALE)
	{
		while(lock_sex==MALE && state_lock==1);
		lock(g);
		women++;
		total_women++;
		if(women > highest_women)
		{
			highest_women = women;
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
	if (women > 0)
	{
		women --;
	}
	else if (men > 0)
	{
		men --;
	}

	unlock();
}
