#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>  

#define P 5 //number of processes, which can be varied
int thread_executions;
int total_elems;
float sum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //allowing one thread in and blocking access to all others

void my_pthread_function(void* arg){

    float aux_sum = 0;

    if(total_elems == P){
        printf("Thread %d executes sum %d\n", (int) arg + 1, (int) arg + 1);
        aux_sum = aux_sum + ((1./ ((int) arg + 1)));
    }
    else if((float) total_elems / P == 0){ //if N(total_elems) is divisible by P
        printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((total_elems/P) * (int) arg) + 1, (total_elems/P) * ((int) arg + 1));
        for(int i = ((total_elems/P) * (int) arg) + 1; i <= (total_elems/P) * ((int) arg + 1); i++){
            aux_sum = aux_sum + ((1./ i));
        }
    }
    else if(total_elems < P){ //the first nth threads execute 1 calculation
        if((int) arg + 1 <= total_elems){
            printf("Thread %d executes sum %d\n", (int) arg + 1, (int) arg + 1);
            aux_sum = aux_sum + ((1./ ((int) arg + 1)));
        }
    }
    else{ //otherwise
        if((int) arg == P - 1){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((total_elems/P) * (int) arg) + 1, total_elems);
            for(int i = ((total_elems/P) * (int) arg) + 1; i <= total_elems; i++){
                aux_sum = aux_sum + ((1./ i));
            }
        }
        else{
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((total_elems/P) * (int) arg) + 1, (total_elems/P) * ((int) arg + 1));
            for(int i = ((total_elems/P) * (int) arg) + 1; i <= (total_elems/P) * ((int) arg + 1); i++){
                aux_sum = aux_sum + ((float) (1./ i));
            }
        }
    }

    pthread_mutex_lock(&mutex);
    sum = sum + aux_sum;
    pthread_mutex_unlock(&mutex);
}

void launch_threads(n_elems){
    pthread_t t_array[P]; //initialize array to store threads/processes
    thread_executions = floor(n_elems/P); //number of executions each process, apart from the last one, will execute. The last thread is responsible for execution the "leftovers" when P doesn't divide N

    for(int i = 0; i < P; i++ ){
        int thr = pthread_create(&t_array[i], NULL, my_pthread_function, i );
        if(thr) {
            printf( stderr, "Error creating thread."); 
            exit(-1);
        }
    }

    for(int i = 0; i < P; i++ ){
        pthread_join(t_array[i], NULL);
    }

    printf("Result: %f\n", sum - log(total_elems));
}

int main(int argc, char *argv[]){
    
    int n_elems; //number of elements of the sum

    if(argc != 2){ //one parameter is missing or there are too many parameters
        printf("To run the program use: ./<file> <number of elements of the sum>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&n_elems);
        printf("The number of elements of the sum: %d\n", n_elems);
        total_elems = n_elems;
        launch_threads(n_elems);
    }

    return 0;
}