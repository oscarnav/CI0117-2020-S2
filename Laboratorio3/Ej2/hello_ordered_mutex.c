#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//Desktop/Paralela/MIREPO/CI0117-2020-S2/Laboratorio3/Ej2

typedef struct {
    pthread_mutex_t* mutex_array;
     size_t total;
} shared_data_t;

typedef struct {
    
    size_t thread_num;
    pthread_mutex_t mutex_m;
    shared_data_t* shared_data;
} mutex_data_t;


void* helloWorld(void* args) {
        
    mutex_data_t* data = (mutex_data_t*) args;
    size_t thread_num = data->thread_num;
    printf("Empezando el thread: %zu\n",thread_num+1 );
    shared_data_t* shared_data = data->shared_data;
    size_t total = shared_data->total;

    pthread_mutex_lock(&shared_data->mutex_array[thread_num]);
    
    if(total==thread_num+1){
     
    }
    else{
     
     pthread_mutex_unlock(&shared_data->mutex_array[thread_num+1]);
     printf("Desbloqueado el thread:%zu\n", thread_num+2 );
    }
     printf("Hello world from thread number # %zu of %zu threads.\n", thread_num+1, total);
     
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
    
    mutex_data_t* mutex_data_list = malloc((size_t)(thread_count * sizeof(mutex_data_t)));
    pthread_mutex_t* mutex_list = malloc((size_t)(thread_count * sizeof(pthread_mutex_t)));
   
    for (size_t i = 0; i < thread_count; ++i) {
    pthread_mutex_init(&mutex_list[i], /*attr*/ NULL);
     mutex_data_list[i].mutex_m = mutex_list[i];
    } 
    
    shared_data->mutex_array = mutex_list;  
    shared_data->total = thread_count;     
     
    
    for (size_t i = 0; i < thread_count; ++i) {
        mutex_data_list[i].thread_num = i;
        mutex_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&mutex_data_list[i]);
    }
     
     for (size_t i = 1; i < thread_count; ++i) {
     pthread_mutex_lock(&mutex_list[i]);

    }

   

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
        //pthread_mutex_unlock(&mutex_data_list[i+1].mutex_m);
       
    }

    printf("Hello world from main thread\n");
    
    
    free(threads);
    free(mutex_list);
    free(shared_data);
    free(mutex_data_list);
    

    return 0;
}