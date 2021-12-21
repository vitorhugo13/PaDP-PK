#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int THREADS;
int dimension;
omp_lock_t l;  

//generate random number between 1 and 9
int generateRandomNumber(int upper, int lower){
    srand(time(0));
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

//Code based on: https://www.geeksforgeeks.org/determinant-of-a-matrix/
void getCofactor(int matrix[dimension][dimension], int temp[dimension][dimension], int p, int q, int dimension){
    int i = 0, j = 0;
    for (int row = 0; row < dimension; row++){
        for (int col = 0; col < dimension; col++){
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
    
    if (dimension == 1){
        adjoint[0][0] = 1;
        return;
    }
 
    int sign = 1, temp[dimension][dimension];

    for (int i=0; i < dimension; i++){
        for (int j = 0; j < dimension; j++)
        {
            //Get cofactor of matrix[i][j]
            getCofactor(matrix, temp, i, j, dimension);
 
            //sign of adjoint[j][i] positive if sum of row and column indexes is even.
            sign = ((i+j) % 2 == 0)? 1: -1;
 
            //Interchanging rows and columns to get the inverse of the cofactor matrix
            adjoint[j][i] = (sign) * (calculateDeterminant(temp, dimension - 1));
        }
    }
}

//Code based on: https://www.geeksforgeeks.org/determinant-of-a-matrix/
int calculateDeterminant(int matrix[dimension][dimension], int dimension){
    int determinant = 0;
 
    if (dimension == 1){
        return matrix[0][0];
    }
 
    int temp[dimension][dimension]; //stores cofactors
    int sign = 1; // store sign multiplier
 
    for (int f = 0; f < dimension; f++){
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
    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
        inverse[i][j] = (adjoint[i][j])/(float)determinant;
        }
    }
}

void findXvalues(double inverse[dimension][dimension], int b[dimension]){
    double results[dimension];

    for(int i = 0; i < dimension; i++){
        double aux = 0;
        for(int j = 0; j < dimension; j++){
            aux = aux + (inverse[i][j] * b[j]);
        }
        results[i] = aux;
    }
    printf("\n--- solutions ---\n");
    for(int i = 0; i < dimension; i++){
        printf("x%d = %f\n", i + 1, results[i]);
    }
}

//Reference: https://www.mathsisfun.com/algebra/systems-linear-equations-matrices.html
void solveEquationSystem(int dimension){
    int matrix[4][4] = {{2, 0, 0, 0},
                        {1, 9, 0, 0},
                        {8, 6, 1, 0},
                        {5, 4, 1, 6}};

    int b[4] = {102, 239, 200, 500 };

    // for(int i = 0; i < dimension; i++){
    //     for(int j = 0; j < dimension; j++){
    //         if(j <= i){
    //             matrix[i][j] = generateRandomNumber(9,1);
    //         }
    //         else{
    //             matrix[i][j] = 0;
    //         }
    //     }

    //     b[i] = generateRandomNumber(100,1);
    // }

    printf("\n---Values matrix---\n");

    int row, columns;
    for (row=0; row<dimension; row++)
    {
        for(columns=0; columns<dimension; columns++)
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
    solveEquationSystem(dimension);

    return 0;
}