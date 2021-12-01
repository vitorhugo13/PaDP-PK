#include <stdio.h>
#include <stdlib.h>
#include "tmeas.h"
#include <pthread.h>


//NOTE: assuming SIZE is divisible by NUM_THR
#define SIZE 1000 //can be varied
#define NUM_THR 10//can be varied
int aggregate_number = (int) SIZE/NUM_THR; //distributes the number of elements equitably per thread


/* define structure of thread's data */
typedef struct {
	int id;
	int *a;
	int *b;
	int *c;
} vectors;

vectors vec;


/*
Function that each thread will have to go through.
Where arg is the thread identifier
arg ∈ [0, NUM_THR [
*/
void* my_pthread_function( void* arg ) 
{

	//Cycle to ensure that each thread runs the right number of aggregations(aggregate_number) and the correct vector element, i.e.
	//Assume size: 15 and num_thr: 3
	//the correct allocation shoul be
	// thread 0 ==> vector element 0,1,2
	// thread 1 ==> vector element 3,4,5
	// thread 2 ==> vector element 6,7,8
	// thread 3 ==> vector element 9,10,11
	// thread 4 ==> vector element 12,13,14
	for(int i = (int) arg * aggregate_number; i < (int) arg * aggregate_number + aggregate_number; i++){ 
		printf("Thread %i ", arg);
		printf("is aggregating vector element %i: ", i);
		vec.c[i]=vec.a[i]+vec.b[i];
		printf("vec.c[%d]=%d\n\n",i,vec.c[i]);
	}
}


int main()
{

	int i;
	double t;
    pthread_t t_array[NUM_THR]; //initialize array to store threads
	
	vec.a=(int *)malloc(sizeof(int)*SIZE);
	vec.b=(int *)malloc(sizeof(int)*SIZE);
	vec.c=(int *)malloc(sizeof(int)*SIZE);
	
	srand(time(NULL));			//random numbers generator initialization;
	
	for(i=0;i<SIZE;i++)
	{
		vec.a[i]=rand()%1001;		//random number generator
		vec.b[i]=rand()%1001;
		printf("vec.a[%d]=%d,vec.b[%d]=%d\n",i,vec.a[i],i,vec.b[i]); //print genetared numbers
	}

	printf("\n");
	tstart();
    
    for( i = 0; i < NUM_THR; i++ )
	{
		int thr = pthread_create( &t_array[i], NULL, my_pthread_function, i ); //create NUM_THR threads
		if( thr ) {
			printf( stderr, "Error creating thread."); //if is there any error creating the thread the programm is aborted
			exit(-1);
		}
	}

    for( i = 0; i < NUM_THR; i++ )	
		pthread_join( t_array[i], NULL); //blocks the calling thread  until the thread whose Join method is called has completed.

	t=tstop();
	printf("time=%lf\n",t);

	return 0;
}
	
	
