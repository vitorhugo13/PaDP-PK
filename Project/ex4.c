#include <stdio.h>
#include <pthread.h>

#define P 5 //number of processes, which can be varied

void launch_threads(int n_elems){
    pthread_t t_array[P]; //initialize array to store threads/processes

    for(int i = 0; i < P; i++ ){
        int thr = pthread_create( &t_array[i], NULL, my_pthread_function, i );
        if(thr) {
            printf( stderr, "Error creating thread."); 
            exit(-1);
        }
    }

    for(int i = 0; i < P; i++ ){
        pthread_join(t_array[i], NULL);
    }
}

void my_pthread_function(void* arg){

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
        launch_threads(n_elems);
    }

    return 0;
}