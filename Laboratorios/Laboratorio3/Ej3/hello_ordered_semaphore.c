#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
//Desktop/Paralela/MIREPO/CI0117-2020-S2/Laboratorio3/Ej2

typedef struct {
    sem_t* sem_array;
    size_t total;
} shared_data_t;

typedef struct {
    size_t thread_num;
    shared_data_t* shared_data;
} sem_data_t;


void* helloWorld(void* args) {
        
    sem_data_t* data = (sem_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    size_t total = shared_data->total;
   
    
    sem_wait(&shared_data->sem_array[thread_num]);
     printf("Hello world from thread number # %zu of %zu threads\n", thread_num+1, total);
    
     
     sem_post(&shared_data->sem_array[thread_num]);
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
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    
    sem_data_t* sem_data_list = malloc((size_t)(thread_count * sizeof(sem_data_t)));
    sem_t* sem_list = malloc((size_t)(thread_count * sizeof(sem_t)));
   
    for (size_t i = 0; i < thread_count; ++i) {
    sem_init(&sem_list[i], 0, 1);
    } 
    
    shared_data->sem_array = sem_list;  
    shared_data->total = thread_count;     
     
    for (size_t i = 1; i < thread_count; ++i) {
     sem_wait(&sem_list[i]);
    } 
    
    for (size_t i = 0; i < thread_count; ++i) {
        sem_data_list[i].thread_num = i;
        sem_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&sem_data_list[i]);
    }
     
     

   

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
        sem_post(&sem_list[i+1]);
       
    }

    printf("Hello world from main thread\n");
    
    
    free(threads);
    free(sem_list);
    free(shared_data);
    free(sem_data_list);
    

    return 0;
}