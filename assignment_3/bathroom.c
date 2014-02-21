// Ted Meyer
// Bathroom.c
// License: GPL v2

#include "bathroom.h"
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <time.h>

//enum gender = 
//{
//	MALE,
//	FEMALE,
//	NONE
//};


//statistics

int highest_men = 0;
int highest_women = 0;
int total_men = 0;
int total_women = 0;


pthread_mutex_t doorsign;
clock_t sim_start;
clock_t sim_last;
unsigned long long avg;


void initialize()
{
	sim_last = clock();
	sim_start = clock();
	avg = 0;
	pthread_mutex_init(&doorsign, NULL);
	printf("initialized\n");
}

void finalize()
{
	unsigned long long total = clock()-sim_start;

	printf("at maximum, there were:\n\tmen:   %i\n\twomen: %i\nand a total or %i men and %i women entered the bathroom\nThere were %llu people on average waiting for the bathroom\n",
		   highest_men, highest_women, total_men, total_women, avg/total);
}




int occupancy = 0;
gender occupant = MALE;

void stats(gender g)
{
	if (g == FEMALE)
	{
		total_women ++;
		if (occupancy > highest_women)
		{
			highest_women = occupancy;
		}
	}
	if (g == MALE)
	{
		total_men ++;
		if (occupancy > highest_men)
		{
			highest_men = occupancy;
		}
	}

	clock_t sim_new = clock();
	avg += occupancy * (sim_new - sim_last);
	sim_last = sim_new;
}


void enter(gender g)
{
	while(1)
	{
		pthread_mutex_lock(&doorsign);

		if (occupancy == 0 || occupant == g)
		{
			occupancy++;
			occupant = g;

			stats(g);

			pthread_mutex_unlock(&doorsign);
			return;
		}

		pthread_mutex_unlock(&doorsign);
	}
}

void leave()
{
	pthread_mutex_lock(&doorsign);
	occupancy --;
	if (occupancy <= 0)
	{
		occupancy = 0;
	}

	stats(-2);

	pthread_mutex_unlock(&doorsign);
}


