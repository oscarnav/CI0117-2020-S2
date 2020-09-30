#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    pthread_mutex_t* mutex_array;
    size_t total;
	size_t delta_x;
	size_t area_total;
} shared_data_t;

typedef struct {
    size_t thread_num;
    shared_data_t* shared_data;
} mutex_data_t;


void* calcularArea(void* args) {
        
    mutex_data_t* data = (mutex_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    size_t total = shared_data->total;
	size_t delta_x = shared_data->delta_x;
	
   
    pthread_mutex_lock(&shared_data->mutex_array[thread_num]);
    
    //Acá cada mutex calcula el area de su cuadrado y lo suma a shared_data->area_total;
    
     
     pthread_mutex_unlock(&shared_data->mutex_array[thread_num]);
    return NULL;
}


int main(int argc, char* arg[]) {



    size_t punto_a = 0;
	size_t punto_b = 0;
	size_t n_cuadrados = 0;
    size_t thread_count = 0;
	
	
    if (argc >= 5) {
        punto_a = (size_t)strtoul(arg[1], NULL, 10);
		punto_b = (size_t)strtoul(arg[2], NULL, 10);
		n_cuadrados = (size_t)strtoul(arg[3], NULL, 10);
		thread_count = (size_t)strtoul(arg[4], NULL, 10);
		
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
    } 
    
    shared_data->mutex_array = mutex_list;  
    shared_data->total = thread_count;     
	shared_data->delta_x = (punto_b-punto_a)/n_cuadrados;
     
	 
    for (size_t i = 1; i < thread_count; ++i) {
     pthread_mutex_lock(&mutex_list[i]);
    } 
    
    for (size_t i = 0; i < thread_count; ++i) {
        mutex_data_list[i].thread_num = i;
        mutex_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, calcularArea, (void*)&mutex_data_list[i]);
    }
     
     

   

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
        pthread_mutex_unlock(&mutex_list[i+1]);
       
    }

   
    
    
    free(threads);
    free(mutex_list);
    free(shared_data);
    free(mutex_data_list);
    

    return 0;
}
