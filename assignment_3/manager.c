#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "bathroom.h"

typedef struct indiv_info
{
	gender g;  // the gender
	int id;	// the thread id
	int loops; // the number of times he/she needs to go
	int stay;  // the length of time he/she goes for
	int arr;   // the ammt of time between each bathroom trip

} indiv_info;


int binary_dist(double mean, double stddev)
{
	double n2 = 0.0;
	int n2_cached = 0;
	if (!n2_cached)
	{
		double x, y, r;
		do 
		{
			x = 2.0*rand()/RAND_MAX - 1;
			y = 2.0*rand()/RAND_MAX - 1;
			r = x*x + y*y;
		} 	while (r == 0.0 || r > 1.0);

		double d = sqrt(-2.0*log(r)/r);
		double n1 = x*d;
		n2 = y*d;
		double result = n1*stddev + mean;
		n2_cached = 1;
		return result;
	} 
	else 
	{
		n2_cached = 0;
		return n2*stddev + mean;
	}
}


void* individual(void* arg)
{
	indiv_info* me = (indiv_info*)(arg);

	int minimum_waiting = 999999999;
	int maximum_waiting = 0;
	int minimum_going   = 999999999;
	int maximum_going   = 0;

	int trips = me -> loops;
	int trips_loop = trips;

	int total_waiting = 0;
	int total_going   = 0;


	while(--trips_loop >= 0)
	{
		int poop = (int)(binary_dist(me->stay, me->stay/2)) * 1000;
		int hold_it = (int)(binary_dist(me->arr, me->arr/2)) * 1000;

		if (poop < 0) poop = -poop;
		if (hold_it < 0) hold_it = -hold_it;

		usleep(hold_it/10);

		clock_t start = clock();
		enter(me -> g);
		
		float waiting_elapsed = clock()-start;

		if (waiting_elapsed > maximum_waiting)
		{
			maximum_waiting = waiting_elapsed;
		}
		if (waiting_elapsed < minimum_waiting)
		{
			minimum_waiting = waiting_elapsed;
		}
		total_waiting += waiting_elapsed;
		total_going   += poop / 1000;
		if (poop > maximum_going)
		{
			maximum_going = poop;
		}
		if (poop < minimum_going)
		{
			minimum_going = poop;
		}
		

		usleep(poop/10);

		leave();
	}


	
	minimum_waiting = (minimum_waiting) / (CLOCKS_PER_SEC / 10000);
	maximum_waiting = (maximum_waiting) / (CLOCKS_PER_SEC / 10000);

	printf("\n╔═══════════════\n║id: %i\n║sex: %s\n║trips: %i\n║min:\n║\twait:%i\n║\tstay:%i\n║max:\n║\twait:%i\n║\tstay:%i\n║avg:\n║\twait:%i\n║\tstay:%i\n╚═══════════════\n", 
			me -> id, (me -> g == MALE) ? "male" : "female", trips,
			minimum_waiting, minimum_going/1000,
			maximum_waiting, maximum_going/1000,
			total_waiting / (trips*100), total_going / trips);

}

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		printf("useage:\n");
		return 0;
	}

	int arg_n_users		= atoi (argv[1]);
	int arg_avg_loop_count = atoi (argv[2]);
	int arg_mean_arrival   = atoi (argv[3]);
	int arg_mean_useage	= atoi (argv[4]);

	int backup_n_user = arg_n_users - 1;

	pthread_t* all_threads = (pthread_t*)(malloc(sizeof(pthread_t) * arg_n_users));


	
	srand(time(NULL));
	initialize();

	while (-- arg_n_users >= 0)
	{
		indiv_info* i = (indiv_info*)(malloc(sizeof(indiv_info)));
					i -> g = (random()%2==0) ? MALE : FEMALE;
					i -> id = arg_n_users;
					i -> loops = (int)(binary_dist(arg_avg_loop_count, arg_avg_loop_count/3)); // make it a little tighter than two
					i -> loops = (i -> loops < 0) ? 1 : i -> loops;
					i -> stay = arg_mean_useage;
					i -> arr = arg_mean_arrival;
		pthread_create(all_threads+arg_n_users, NULL, individual, i);
	}


	while(backup_n_user >= 0)
	{
		pthread_join(*(all_threads+backup_n_user), NULL);
		backup_n_user --;
	}

	finalize();
	return 0;
}