#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) { 
    int rank, ranksent, size, source, dest, tag, i, len, number ;
    char name[20];
    len = 20;
    MPI_Status status; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);

    do {

        if(rank != 0) { 

            MPI_Recv(&number, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
            printf("Process %d got %d from process %d\n", rank, number, status.MPI_SOURCE);
            MPI_Send(&number, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            
        } else { 
            
            printf("Enter a number:\n");
            fflush(stdout);
            scanf("%d", &number);
            printf("Process 0 got a %d\n", number);

            MPI_Send(&number, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            if( rank == 0){
                
                MPI_Recv(&number, 1, MPI_INT, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
            
            }
        } 

    } while(number >= 0);

    MPI_Finalize(); 
    return(0); 
}