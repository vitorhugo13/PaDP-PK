#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) 
{ 

    int rank,ranksent,size,source,dest,tag,i,len;
    char name[20];
    len=20;

    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);


    // fundamental variables for non-blocking operations
    MPI_Status status; 
    MPI_Request request; 


    int *input_number = (int*) malloc(sizeof(int));
    *input_number = 0;
    
    while(*input_number >= 0){

        if(rank != 0){ //receiving number from rank before 
            MPI_Irecv(input_number,1,MPI_INT,(rank - 1),MPI_ANY_TAG,MPI_COMM_WORLD,&request); 
            MPI_Wait(&request, &status);
            printf("Process %d got %d from process %d\n",rank, *input_number, status.MPI_SOURCE);
        }
        else{ //enter a new number at rank 0
            printf("\n");
            printf("Enter a number: \n");
            scanf("%d", input_number);
            printf("Process 0 got a %d\n", *input_number);
        }

        //sending number to next node in ring
        if(rank + 1 == size){ //means we need to send the value to the start of the ring
            MPI_Isend(input_number,1,MPI_INT,0,tag,MPI_COMM_WORLD, &request);
        }
        else{
            MPI_Isend(input_number,1,MPI_INT,rank + 1,tag,MPI_COMM_WORLD, &request);
        }

        if(rank == 0){ //apart from iteraction 1 we also need to receive the value in rank 0
            MPI_Irecv(input_number,1,MPI_INT,rank - 1,MPI_ANY_TAG,MPI_COMM_WORLD,&request); 
            MPI_Wait(&request, &status);
            printf("Process %d got %d from process %d\n",rank, *input_number, status.MPI_SOURCE); 
        }
    }

    MPI_Finalize(); 
    return(0); 
}