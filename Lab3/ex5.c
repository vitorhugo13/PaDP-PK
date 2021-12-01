#include<stdio.h>
#include<omp.h>

threads = 4; 
chunk_size = 3; //to use this value you just need to uncomment and add the variable as a second argument to schedule()
size = 15000 ; 

int main(){

    //for time measurement we'll use omp_get_wtime as presented in the lecture

    float start_time = omp_get_wtime();
    omp_set_num_threads(threads); 
    int a = 50000;

    #pragma omp parallel for schedule(dynamic) //static or dynamic
    for(int i=0; i < size; i++) 
    { 
        a*=0.5; 
    }

    float end_time = omp_get_wtime();
    printf("Time = %f\n", end_time - start_time);

}

/*
static 3 - 0.023438s
static default - 0.011719s
dynamic default - 0.011719s
dynamic 3 - 0.027344s
*/