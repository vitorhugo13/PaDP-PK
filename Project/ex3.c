#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x, n; //variables in the equation
int THREADS; //threads that will be used to parallelize operations
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; ////allowing one thread in and blocking access to all others

void my_pthread_function(void* arg){

}

void launch_threads(){

    pthread_t t_array[THREADS]; //initialize array to store threads/processes

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
    }

    return 0;
}