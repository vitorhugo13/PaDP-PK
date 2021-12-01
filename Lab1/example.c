#include <stdio.h>		/* need for fflush() */
#include <stdlib.h>		/* need for exit() */
#include <pthread.h>
#include "tmeas.h"

/* define the number of threads to create */
#define NUM_THREADS	20
#define WAIT 1000000

void* my_pthread_function( void *ptr );

main()
{
	pthread_t thread_array[NUM_THREADS];
     	int i, thr;
	
    tstart();
	for( i = 0; i < NUM_THREADS; i++ )
	{
		thr = pthread_create( &thread_array[i], NULL, my_pthread_function, (void *)i );
		if( thr ) {
			fprintf( stderr, "Error: return code from pthread_create() - %d", thr);
			exit(-1);
		}
	}

	/* Wait till threads are complete before main continues. Unless we  */
     	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	for( i = 0; i < NUM_THREADS; i++ )	
		pthread_join( thread_array[i], NULL);

        double t;
        t = tstop();
        printf("Time: %lf",t); 
	
	return EXIT_SUCCESS;    
}

void* my_pthread_function( void *thr_data )
{
	int i,id;
	id=(int)thr_data;
	printf("I am %d thread\n", id);	
	//busy waiting
	while(i<WAIT) i++;	
}


// Q: What can you notice after several runs? 
// A: Threads are not sequential, so the order of lunch will vary from time to time, 
// that is, there is no order. As sequence, the threads won't finish at the same time and with no order. We are at the presence of race conditions.
// Sometimes there are some program executions that take way longer than others.