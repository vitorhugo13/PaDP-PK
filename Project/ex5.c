#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int THREADS;

//NOTE: in order to test the programm with a different number of dimensions, the next 3 variables(dimension, matrix, b) should be changed accordingly.
int dimension = 4;
int matrix[4][4] = {{9, 0, 0, 0},
                    {7, 2, 0, 0},
                    {2, 6, 4, 0},
                    {5, 4, 3, 2}};
int b[4] = {200, 400, 333, 1212};

omp_lock_t l;  

//Code based on: https://www.geeksforgeeks.org/determinant-of-a-matrix/
void getCofactor(int matrix[dimension][dimension], int temp[dimension][dimension], int p, int q, int dimension){
    int i = 0, j = 0;
    int row, col;

    for (row = 0; row < dimension; row++){
        for (col = 0; col < dimension; col++){
            if (row != p && col != q){
                temp[i][j++] = matrix[row][col];
                if (j == dimension - 1){
                    j = 0;
                    i++;
                }
            }
        }
    }
}

void getAdjoint(int matrix[dimension][dimension], int adjoint[dimension][dimension]){
    int i, j;

    if (dimension == 1){
        adjoint[0][0] = 1;
        return;
    }
 
    int sign = 1, temp[dimension][dimension];

    #pragma omp parallel for shared(dimension, i, adjoint) private(j)
    for (i=0; i < dimension; i++){
        omp_set_lock(&l);

        for (j = 0; j < dimension; j++)
        {
            //Get cofactor of matrix[i][j]
            getCofactor(matrix, temp, i, j, dimension);
 
            //sign of adjoint[j][i] positive if sum of row and column indexes is even.
            sign = ((i+j) % 2 == 0)? 1: -1;
 
            //Interchanging rows and columns to get the inverse of the cofactor matrix
            adjoint[j][i] = (sign) * (calculateDeterminant(temp, dimension - 1));
        }
        omp_unset_lock(&l);
    }
}

//Code based on: https://www.geeksforgeeks.org/determinant-of-a-matrix/
int calculateDeterminant(int matrix[dimension][dimension], int dimension){
    int determinant = 0;
    int f;

    if (dimension == 1){
        return matrix[0][0];
    }
 
    int temp[dimension][dimension]; //stores cofactors
    int sign = 1; // store sign multiplier
 
    for (f = 0; f < dimension; f++){
        getCofactor(matrix, temp, 0, f, dimension);
        determinant = determinant + sign * matrix[0][f] * calculateDeterminant(temp, dimension - 1);
        sign = -sign;
    }
 
    return determinant;
}

//Reference: https://www.thecrazyprogrammer.com/2017/02/c-c-program-find-inverse-matrix.html
void calculateInverse(int matrix[dimension][dimension], int dimension, double inverse[dimension][dimension]){
    int i, j;
    int determinant = 0;
    determinant = calculateDeterminant(matrix, dimension);

    //finding determinant
    printf("\nMatrix Determinant: %d\n", determinant);

    //finding adjoint
    int adjoint[dimension][dimension];
    getAdjoint(matrix, adjoint);

    //finding inverse
    #pragma omp parallel for schedule(static) shared(i, dimension, adjoint, inverse) private(j)
    for (i = 0; i < dimension; i++){
        omp_set_lock(&l);
        for (j = 0; j < dimension; j++){
            inverse[i][j] = (adjoint[i][j])/(float)determinant;
        }
        omp_unset_lock(&l);
    }
}

void findXvalues(double inverse[dimension][dimension], int b[dimension]){
    double results[dimension];

    #pragma omp parallel for schedule(static) shared(results)
    for(int i = 0; i < dimension; i++){
        omp_set_lock(&l);
        double aux = 0;
        for(int j = 0; j < dimension; j++){
            aux = aux + (inverse[i][j] * b[j]);
        }
        results[i] = aux;
        omp_unset_lock(&l);
    }
    printf("\n--- solutions ---\n");
    for(int i = 0; i < dimension; i++){
        printf("x%d = %f\n", i + 1, results[i]);
    }
}

//Reference: https://www.mathsisfun.com/algebra/systems-linear-equations-matrices.html
void solveEquationSystem(int dimension){
    printf("\n---Values matrix---\n");

    int row, columns;
    for (row=0; row<dimension; row++)
    {
        for(columns=0; columns < dimension; columns++)
        {
            printf("%d     ", matrix[row][columns]);
        }
        printf("\n");
    }

    printf("\n---b values---\n");
    for(int i = 0; i < dimension; i++){
        printf("%d     ", b[i]);
    }
    printf("\n");


    double inverse[dimension][dimension];
    calculateInverse(matrix, dimension, inverse);

    printf("\n---Values inverse---\n");
    for (row=0; row < dimension; row++)
    {
        for(columns=0; columns < dimension; columns++)
        {
            printf("%f     ", inverse[row][columns]);
        }
        printf("\n");
    }

    findXvalues(inverse, b);
}

void displayEquationSystem(int dimension){
    printf("The system of equations to be solved will be as follows\n");

    for(int i = 0; i < dimension; i++){
        printf("> ");

        for(int j = 0; j < i + 1; j++){
            if(j < i ){
                printf("a(%d,%d)x%d + ", i+1, j+1, j+1);
            }
            else{
                printf("a(%d,%d)x%d", i+1, j+1, j+1);
            }
        }

        printf(" = b%d\n", i + 1);
    }
}


int main(int argc, char *argv[]){
    
    if(argc != 2){ //one parameter is missing or there are too many parameters
        printf("To run the program use: ./<file>  <number of threads>\n");
        return -1;
    }
    else{
        sscanf(argv[1],"%d",&THREADS);
        printf("Threads to be used: %d\n\n", THREADS);
    }

    omp_init_lock(&l);
	omp_set_num_threads(THREADS);
    displayEquationSystem(dimension);
    solveEquationSystem(dimension);
    omp_destroy_lock(&l);

    return 0;
}