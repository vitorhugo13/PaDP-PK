#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int limit;
int THREADS;
omp_lock_t l;  

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("To run the program use: ./<file> <upper limit> <number of threads>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&limit);
        sscanf(argv[2],"%d",&THREADS);
        printf("We will calculate the prime numbers up to: %d\n", limit);
        printf("Threads to be used: %d\n", THREADS);
    }

    return 0;
}