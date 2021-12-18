#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <pthread.h>

#define THREADS 8
int n_subintervals; //number of elements of the sum
int a = 0, b = 1;
float h, sum = 0; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //allowing one thread in and blocking access to all others

double f(double x){
    return 1./(1 + pow(x, 2));
}

double xi(int i){
    return a + i * h;
}

void my_pthread_function(void* arg){

    float aux_sum = 0;

    if(n_subintervals == THREADS){
        if((int) arg != 0 && (int) arg != n_subintervals - 1){
            printf("Thread %d executes sum %d\n", (int) arg + 1, (int) arg + 1);
            aux_sum = aux_sum + f(xi(((int) arg + 1)));
        }
    }
    else if((float) n_subintervals / THREADS == 0){ //if n_subintervals is divisible by THREADS
        if((int) arg != 0 && (int) arg != n_subintervals - 1){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 1, (n_subintervals/THREADS) * ((int) arg + 1));
            for(int i = ((n_subintervals/THREADS) * (int) arg) + 1; i <= (n_subintervals/THREADS) * ((int) arg + 1); i++){
                aux_sum = aux_sum + f(xi(i));
            }
        }
        else if((int) arg == 0){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 2, (n_subintervals/THREADS) * ((int) arg + 1));
            for(int i = ((n_subintervals/THREADS) * (int) arg) + 2; i <= (n_subintervals/THREADS) * ((int) arg + 1); i++){
                aux_sum = aux_sum + f(xi(i));
            }
        }
        else if((int) arg == n_subintervals - 1){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 1, (n_subintervals/THREADS) * ((int) arg ));
            for(int i = ((n_subintervals/THREADS) * (int) arg) + 1; i <= (n_subintervals/THREADS) * ((int) arg ); i++){
                aux_sum = aux_sum + f(xi(i));
            }
        }
    }
    else{ //otherwise
        if((int) arg == THREADS - 1){
            printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 1, n_subintervals - 1);
            for(int i = ((n_subintervals/THREADS) * (int) arg) + 1; i < n_subintervals; i++){
                aux_sum = aux_sum + f(xi(i));
            }
        }
        else{
            if((int) arg != 0 && (int) arg != n_subintervals - 1){
                printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 1, (n_subintervals/THREADS) * ((int) arg + 1));
                for(int i = ((n_subintervals/THREADS) * (int) arg) + 1; i <= (n_subintervals/THREADS) * ((int) arg + 1); i++){
                    aux_sum = aux_sum + f(xi(i));
                }
            }
            else if((int) arg == 0){
                printf("Thread %d executes sum from %d to %d\n", (int) arg + 1, ((n_subintervals/THREADS) * (int) arg) + 2, (n_subintervals/THREADS) * ((int) arg + 1));
                for(int i = ((n_subintervals/THREADS) * (int) arg) + 2; i <= (n_subintervals/THREADS) * ((int) arg + 1); i++){
                    aux_sum = aux_sum + f(xi(i));
                }
            }
        }
    }

    pthread_mutex_lock(&mutex);
    sum = sum + aux_sum;
    pthread_mutex_unlock(&mutex);
}

void launch_threads(n_elems){
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

    sum = sum * h;
    sum = sum * 4; //because the initial integral is multiplied by 4
    printf("Result: %f\n", sum);
    
}

int main(int argc, char *argv[]){

    if(argc != 2){ //one parameter is missing or there are too many parameters
        printf("To run the program use: ./<file> <number of subintervals of integration>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&n_subintervals);
        if(n_subintervals < THREADS){
            printf("The number of subintervals of integration must be >= %d\n", THREADS);
            return -1;
        }
        printf("The number of subintervals of integration: %d\n", n_subintervals);
        h = ((b-a)/(float) n_subintervals);
        sum = (f(a) + f(b)) / (float) 2;
        printf("sum0:%f\n", sum);

        launch_threads();
    }

    return 0;
}