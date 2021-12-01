#include "mpi.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    printf("\n");
    int rank, size;
    int age;
    int i;
    char name[20];
    char buffer[110];
    char address[30];
    int id_number;
    char nationality[30];

    int position = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2)
    {
        printf("Please run with 2 processes.\n");
        fflush(stdout);
        MPI_Finalize();
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0){

        for (i=0; i<20; i++){
            name[i] =  (rand() % (121 - 97 + 1)) + 97;
        }

        age = 123;

        for (i=0; i < 30; i++){
            address[i] = (rand() % (121 - 97 + 1)) + 97;
        }

        id_number = 123456789;

        for (i=0; i < 30; i++){
            nationality[i] =  (rand() % (121 - 97 + 1)) + 97;
        }

        MPI_Pack(&age, 1, MPI_INT, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Pack(name, 20, MPI_CHAR, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Pack(address, 30, MPI_CHAR, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Pack(&id_number, 1, MPI_INT, buffer, 110, &position, MPI_COMM_WORLD);
        MPI_Pack(nationality, 30, MPI_CHAR, buffer, 110, &position, MPI_COMM_WORLD);

        //Time measurement
        float start_time = MPI_Wtime();
        MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
        float end_time = MPI_Wtime();
        printf("Total time for sending data: %fs. \n\n", end_time -  start_time);

    }

    if (rank == 1){
        MPI_Recv(buffer, 110, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
        
        MPI_Unpack(buffer, 110, &position, &age, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 110, &position, name, 20, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 110, &position, address, 30, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 110, &position, &id_number, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 110, &position, nationality, 30, MPI_CHAR, MPI_COMM_WORLD);
        printf("name[0] = %c...name[19] = %c\n", (char)name[0], (char)name[19]);
        printf("age=%d\n", age);
        printf("address[0] = %c...address[29] = %c\n", (char)address[0], (char)address[29]);
        printf("id_number=%d\n", id_number);
        printf("nationality[0] = %c...nationality[29] = %c\n", (char)nationality[0], (char)nationality[29]);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
} 
