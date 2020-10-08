#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include dna_sequences.h
//Desktop/Paralela/MIREPO/CI0117-2020-S2/Laboratorio3/Ej2

typedef struct{
	char letra;
	size_t frecuencia;
}dna_data

typedef struct {
    pthread_mutex_t mutex;
    size_t total;
	dna_data* secuence1_array;
	dna_data* secuence2_array;
	dna_data* common_array;
} shared_data_t;

typedef struct {
    size_t thread_num;
    shared_data_t* shared_data;
} pthread_data_t;





void* ocurSecuencia1(void* args) {
      thread_data_t* data = args; 
      size_t thread_num = data->thread_num;
	  shared_data_t* shared_data = data->shared_data;
	  
	  pthread_mutex_lock(&shared_data->mutex);
	   for(int i=0;i<DNA1_len;++i){
		   
		    for(int i2=0;i2<26;++i2){
				
				if(shared_data->secuence1_array[i2]->letra == DNA1[i]){
					++shared_data->secuence1_array[i2]->frecuencia;
					
				}
				
				
		    }
			
	   }
	  
	  pthread_mutex_unlock(&shared_data->mutex);
	  
	  
    return NULL;
}

void* ocurSecuencia2(void* args) {
      thread_data_t* data = args; 
      size_t thread_num = data->thread_num;
	  shared_data_t* shared_data = data->shared_data;
    
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
    
	/*crear arreglos*/
	pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    thread_data_t* thread_data = malloc((size_t)(thread_count * sizeof(thread_data_t)));
	dna_data* dna_data_list = malloc((size_t)(27)));     
	
	//Para inicializar el vector de letras
    int ia;
    for(ia=0; i<26; i++){
        dna_data_list[ia]->letra = 'a'+ia;
        dna_data_list[ia]->frecuencia = 0;
	
	}
	    dna_data_list[ia]='\0';
	
   
    
	pthread_mutex_init(&shared_data->mutex, /*attr*/ NULL);
    
    //Asignar el shared_data correctamente
    shared_data->secuence1_array = dna_data_list;  
	shared_data->secuence2_array = dna_data_list;
    shared_data->common_array = dna_data_list;
	shared_data->total = thread_count;     
     
    
    //Creacion de los hilos
    for (size_t i = 0; i < thread_count; ++i){
        thread_data[i].thread_num = i;
        thread_data[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, ocurSecuencia1, (void*)&thread_data[i]);
    }
     
    
    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
       
    }
	
    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
       
    }
	 for (size_t i = 0; i < 26; ++i) {
        if(shared_data->secuence1_array[i]->frecuencia!=0){
			printf(" %c :%zu "shared_data->secuence1_array[i]->letra,shared_data->secuence1_array[i]->frecuencia);	 
		}	
       
    }
    
    
    
    free(threads);
    free(dna_data_list);
    free(shared_data);
    
    

    return 0;
}