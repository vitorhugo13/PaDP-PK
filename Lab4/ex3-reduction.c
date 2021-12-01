#include<stdio.h>
#include<omp.h>

#define N 500

int main()
{
   int square=0, number, i;
	
   printf("Insert number:\n");
   scanf("%d",&number);

   float start_time = omp_get_wtime(); 

   //reduction(op:var)
   #pragma omp parallel for reduction(+:square)  //reduction clause
   for(i=0; i < N ; i++)
   {
	   square+=number*number;
   }


   printf("Execution time = %f\n", omp_get_wtime() - start_time); 
   printf("Square=%d\n",square);
}
