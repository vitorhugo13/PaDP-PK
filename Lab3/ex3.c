#include<stdio.h>
#include<omp.h>

threads = 4; 
// chunk_size = 3; //to use this value you just need to uncomment and add the variable as a second argument to schedule()
size = 15 ; 

int main(){

    omp_set_num_threads(threads); 
    int a = 50000;

    #pragma omp parallel for schedule(static) //static or dynamic
    for(int i=0; i < size; i++) 
    { 
        a*=0.5; 
        printf("Thread %d, index %d, a = %d\n",omp_get_thread_num(),i,a); 
    }

    printf("After execution of iterations 'a' = %d\n",a); 

}