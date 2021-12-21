#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int THREADS;
int dimension;


void displayEquationSystem(int dimension){
    printf("The system of equations to be solved will be as follows\n");
    for(int i = 0; i < dimension; i++){
        printf("> ");
        for(int j = 0; j < i + 1; j++){
            if(j < i ){
                printf("a%d%dx%d + ", i+1, j+1, j+1);
            }
            else{
                printf("a%d%dx%d", i+1, j+1, j+1);
            }
        }

        printf(" = b%d\n", i + 1);
    }
}


int main(int argc, char *argv[]){
    
    if(argc != 3){ //one parameter is missing or there are too many parameters
        printf("To run the program use: ./<file> <number of x_variables(system's dimension)> <number of threads>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&dimension);
        sscanf(argv[2],"%d",&THREADS);
        printf("System's dimension: %d\n", dimension);
        printf("Threads to be used: %d\n\n", THREADS);
    }

    displayEquationSystem(dimension);

    return 0;
}