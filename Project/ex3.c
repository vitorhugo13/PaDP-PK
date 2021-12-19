#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "tmeas.h"

int x, n; //variables in the equation
int THREADS; //threads that will be used to parallelize operations
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; ////allowing one thread in and blocking access to all others
long double sum = 0;
long double e = 2.7182818284590452353602874713527;

void my_pthread_function(void* arg){
    long double aux_sum = 0;

    if(n == THREADS){
        printf("Thread %d executes sum %d\n", (int) arg + 1, (int) arg + 1);
        long double exponential = pow(x, (int)(arg + 1));
        aux_sum = exponential;
        for(int i = (int) arg + 1; i > 0; i--){
            aux_sum = aux_sum / (float) i;
        }
    }
    else{ //otherwise
        if((int) arg == THREADS - 1){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n/THREADS) * (int) arg) + 1, n);
            for(int i = ((n/THREADS) * (int) arg) + 1; i <= n; i++){
                long double exponential = pow(x, i);
                long double aux_sum2 = exponential;

                for(int j = i; j > 0; j--){
                    aux_sum2 = aux_sum2 / (float) j;
                }

                aux_sum = aux_sum + aux_sum2;
            }
        }
        else{
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n/THREADS) * (int) arg) + 1, (n/THREADS) * ((int) arg + 1));
            for(int i = ((n/THREADS) * (int) arg) + 1; i <= (n/THREADS) * ((int) arg + 1); i++){
                long double exponential = pow(x, i);
                long double aux_sum2 = exponential;

                for(int j = i; j > 0; j--){
                    aux_sum2 = aux_sum2 / (float) j;   
                }

                aux_sum = aux_sum + aux_sum2;
            }
        }
    }

    pthread_mutex_lock(&mutex);
    sum = sum + aux_sum;
    pthread_mutex_unlock(&mutex);
}

void launch_threads(){

    pthread_t t_array[THREADS]; //initialize array to store threads/processes

	tstart();

    for(int i = 0; i < THREADS; i++ ){
        int thr = pthread_create(&t_array[i], NULL, my_pthread_function, i );
        if(thr) {
            printf( stderr, "Error creating thread."); 
            exit(-1);
        }
    }

    for(int i = 0; i < THREADS; i++ ){
        pthread_join(t_array[i], NULL);
    }

    float time = tstop();
    printf("----- Using the Mclaurin series -----\n");
    printf("Result: %Le\n", sum);
    printf("Execution time: %f s\n", time);
}

int main(int argc, char *argv[]){

    if(argc != 4){ //one parameter is missing or there are too many parameters
        printf("To run the program use: ./<file> <x_value> <n_value> <number of threads>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&x);
        sscanf(argv[2],"%d",&n);
        sscanf(argv[3],"%d",&THREADS);

        if(n < THREADS){
            printf("The n_value must be >= %d\n", THREADS);
            return -1;
        }

        printf("x_value: %d\n", x);
        printf("n_value: %d\n", n);
        printf("The number of threads being used: %d\n", THREADS);
        launch_threads();

        printf("----- Value from the math library-----\n");
        printf("Result: %f\n", pow(e, x));
    }

    return 0;
}