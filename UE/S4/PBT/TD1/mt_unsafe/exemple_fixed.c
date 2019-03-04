#include <pthread.h>
#include <stdio.h>


#define COUNT_PER_THREAD 10000
#define NUM_THREADS 10

volatile unsigned long count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* ajout */

void *  parallel_count(void *args){
   int i;
   
   pthread_mutex_lock(&mutex); /* ajout */
   for(i=0; i < COUNT_PER_THREAD; ++i){
      ++count;
   }
   pthread_mutex_unlock(&mutex); /* ajout */
   
   return NULL;
}



int main(int argc, char** argv)
{
   int i;
   pthread_t pid[NUM_THREADS];
   
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_create(&pid[i], NULL, parallel_count, NULL);
   }
   
   for(i = 0; i < NUM_THREADS; ++i){
      pthread_join(pid[i], NULL);
   }

   pthread_mutex_destroy(&mutex);
   
   printf("Valeur finale de count %ld \n", count); /* ajout */
   
   return 0;
}
