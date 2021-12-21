#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <math.h>

int limit;
int THREADS;
omp_lock_t l;  

void retrievePrimes(){
    bool aux[limit]; //acts as an auxiliar array to the algorithm 

    omp_init_lock(&l);
	omp_set_num_threads(THREADS);
    int i;

    #pragma omp parallel for shared(i)
    for (i = 0; i <= limit; i++){
        aux[i] = true;
    }

    int p, j;
    #pragma omp parallel for shared(p, aux, limit) private(j)
    for (p = 2; p <= limit; p++){
        if (aux[p]){
            for (j = pow(p, 2); j <= limit; j = j + p){
                aux[j] = false;
            }
        }
    }

    omp_destroy_lock(&l);

    printf("Primes until %d: [", limit);
    for (int i = 2; i <= limit; i++){
        if (aux[i] == true){
            printf("%d ", i);
        }
    }
    printf("]\n");

}

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("To run the program use: ./<file> <upper limit> <number of threads>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&limit);
        sscanf(argv[2],"%d",&THREADS);
        printf("We will calculate the prime numbers up to: %d\n", limit);
        printf("Threads to be used: %d\n\n", THREADS);
    }

    int primes[limit-1];

    for (int i = 2; i <= limit; i++){
        primes[i - 2] = i;
    }

    printf("Initial list: [");
    for(int j = 0; j < sizeof(primes)/sizeof(int); j++){
        if(j == sizeof(primes)/sizeof(int) - 1){
            printf("%d", primes[j]);
        }
        else{
            printf("%d,", primes[j]);
        }
    }
    printf("]\n\n");

    retrievePrimes();

    return 0;
}