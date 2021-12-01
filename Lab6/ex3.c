#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include <time.h>

typedef struct {
	char name[20];
	int age;
    char address[100];
    int id_number;
    char nationality[30];
} Data;


int main(int argc, char **argv)
{
    printf("\n");
	int rank, size, r;
	int src, dst, tag, i;	
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[5] = { MPI_CHAR, MPI_INT, MPI_CHAR, MPI_INT, MPI_CHAR}; //added the new 3 elements
	int blen[5] = { 20, 1, 100, 1, 30 };
	MPI_Aint disp[5];
	MPI_Aint base, addr;
	Data tabrecord,tabrecieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0){
		printf("MPI_Type_create_struct()\n");
    }

	MPI_Get_address(&tabrecord, &base);

	MPI_Get_address(&(tabrecord.name), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(tabrecord.age), &addr);
	disp[1] = addr - base;
    //for the new 3 elements of the struct
	MPI_Get_address(&(tabrecord.address), &addr);
	disp[2] = addr - base;
	MPI_Get_address(&(tabrecord.id_number), &addr);
	disp[3] = addr - base;
    MPI_Get_address(&(tabrecord.nationality), &addr);
	disp[4] = addr - base;

	MPI_Type_create_struct(5, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);

	if(rank != 0){
		strcpy(tabrecieved.name, "Vitor Hugo");
		tabrecieved.age = 22;
		strcpy(tabrecieved.address, "Street Teatro Cubano et Banialuka");
        tabrecieved.id_number = 192837465;
		strcpy(tabrecieved.nationality, "polish");
		printf("In process %d my name is %s and my age is %d, I live in %s, my id_number is %d and I'm %s.\n",rank,tabrecieved.name,tabrecieved.age, tabrecieved.address, tabrecieved.id_number, tabrecieved.nationality);	

		MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("In process %d I have received name: %s ",rank,tabrecieved.name);
		printf(" and age: %d",tabrecieved.age);
		printf(" and address: %s",tabrecieved.address);
		printf(" and id_number: %d",tabrecieved.id_number);
		printf(" and nationality: %s\n",tabrecieved.nationality);
	}
	else{
		strcpy(tabrecord.name, "Joao Pedro");
		tabrecord.age = 14;
		strcpy(tabrecord.address, "Street HukiMuki et La Bodega");
		tabrecord.id_number = 987654321;
		strcpy(tabrecieved.nationality, "polish");

        //Time measurement
        float start_time = MPI_Wtime();
		for (r=1; r<size; r++){
			MPI_Send(&tabrecord, 1, new_type, r, 0, MPI_COMM_WORLD);
        }
        float end_time = MPI_Wtime();
        printf("Total time for sending data: %fs. \n\n", end_time -  start_time);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

