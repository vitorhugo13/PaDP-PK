#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 

int main(int argc, char **argv){

    int rank, size, len;
    char name[20];
    len=20;

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name, &len);


    int* input_number = (int*) malloc(sizeof(int));
    *input_number = 0;

    while(*input_number >= 0){

        if(rank == 0){
            printf("Enter a number: \n");
            scanf("%d", input_number);
        }

        MPI_Bcast(input_number, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Process %d, received %d after broadcast. \n",rank, *input_number); 
        
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize(); 
    return(0);
}