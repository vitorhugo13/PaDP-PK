#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>     // sleep
#include <stdbool.h>

/* All these variables can be varied */
#define N_CLIENTS 20   
#define N_BASKETS 12
#define MAXIMUM_TIME 8
#define N_VISITS 2

pthread_mutex_t basket[N_BASKETS];
pthread_mutex_t mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond    = PTHREAD_COND_INITIALIZER; 
char condition = 0;
int terminate = 0;
int basket_array[N_BASKETS];
int visits_array[N_CLIENTS];

void* thread(void* number) {
        srand(time(NULL));   
        int r = rand() % (N_BASKETS);


        for(int visit = 0; visit < N_VISITS; visit++){ //assuming all the clients only make N_VISITS

                while(basket_array[r] != 0){ //client randomly picks a free basket (0 means free)
                        r = rand() % (N_BASKETS); //allocated basket
                }

                pthread_mutex_lock(&mutex);
                basket_array[r] = 1; //modifies the value of basket r to 1, meaning it's being used.
                pthread_mutex_unlock(&mutex);

                if(basket_array[r] == 1){

                        pthread_mutex_lock(&basket[r]);
                        printf("\tClient no.%d is taking shopping cart no. %d\n", (int)number, (r+1));
                        int waiting_time = rand() % MAXIMUM_TIME + 1;
                        sleep(waiting_time);
                        printf("\tClient no. %d left the shop after %d  time units releasing the basket no. %d \n", (int)number, waiting_time, r+1);
                        pthread_mutex_unlock(&basket[r]);

                        pthread_mutex_lock(&mutex);
                        basket_array[r] = 0; //when the client leaves the shop, basket r stays free again
                        pthread_mutex_unlock(&mutex);
                }

                sleep(1);
        }

        return NULL;
}
 
int main() {
        pthread_t id[N_CLIENTS];
        puts("Beginning of the program");
 
        for (int i=0; i < N_CLIENTS; i++) {
                errno = pthread_create(&id[i], NULL, thread, (void*)(i+1));
                if (errno) {
                        perror("pthread_create");
                        return EXIT_FAILURE;
                }
        }

        for(int i = 0; i < N_BASKETS; i++){
                pthread_mutex_init(&basket[i], NULL);
        }

        for(int i = 0; i < N_CLIENTS; i++ ){	
		pthread_join( id[i], NULL); //blocks the calling thread  until the thread whose Join method is called has completed
        }

        puts("End of program");
        return EXIT_SUCCESS;
}

