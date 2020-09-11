#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct {
    size_t thread_num;
    pthread_mutex_t mutex_m;
} mutex_data_t;


void* helloWorld(void* args) {

    mutex_data_t* data = (mutex_data_t*) args;
    size_t thread_num = data->thread_num;
    printf("Hello world from thread number # %zu.\n", thread_num);
    return NULL;
}


int main(int argc, char* arg[]) {

    size_t thread_count = 0;
    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }
    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    mutex_data_t* mutex_data_list = malloc((size_t)(thread_count * sizeof(mutex_data_t)));

   
    for (size_t i = 0; i < thread_count; ++i) {
    pthread_mutex_init(&mutex_data_list[i].mutex_m, /*attr*/ NULL);
    
    } 


    for (size_t i = 0; i < thread_count; ++i) {
        mutex_data_list[i].thread_num = i;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&mutex_data_list[i]);
    }
     

     for (size_t i = 1; i < thread_count; ++i) {
        pthread_mutex_lock(&mutex_data_list[i].mutex_m);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
        pthread_mutex_unlock(&mutex_data_list[i+1].mutex_m);
    }

    printf("Hello world from main thread\n");

    free(threads);
    free(mutex_data_list);
    

    return 0;
}