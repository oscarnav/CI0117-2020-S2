#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void* helloWorld(void* args) {
     
    return NULL;
}


int main(int argc, char* arg[]) {

    // arg[0] = command
    // arg[1] = first param

    
    size_t T = 0;
    
    if (argc >= 2) {

       T = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        T = (size_t)1;
    }
    

    long nanos_menor = 0;

    for (size_t i = 0; i < T; ++i) {
        struct timespec before;
        clock_gettime(CLOCK_REALTIME, &before);

        pthread_t thread;
        pthread_create(&thread, NULL, helloWorld, NULL);
        pthread_join(thread, NULL);
        
        struct timespec after;
        clock_gettime(CLOCK_REALTIME, &after);
        if(i==0 || nanos_menor > after.tv_nsec-before.tv_nsec){
        nanos_menor = after.tv_nsec-before.tv_nsec;
        }
        
    }
     
     //float conv = 1e9;
    printf("Minimum thread creation and destruction time was: %fs, among %ld trials\n", nanos_menor/1e9, T ); 
    return 0;
}
