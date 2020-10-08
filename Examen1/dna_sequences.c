#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dna_sequences.h"
//Desktop/Paralela/MIREPO/CI0117-2020-S2/Laboratorio3/Ej2

typedef struct{
	char letra;
	size_t frecuencia;
}dna_data_t;

typedef struct {
    pthread_mutex_t* mutex_array;
    size_t total;
    size_t actual;
	dna_data_t* secuence1_array;
	dna_data_t* secuence2_array;
	dna_data_t* common_array;
} shared_data_t;

typedef struct {
    size_t cant_lugares;
    size_t thread_num;
    shared_data_t* shared_data;
} thread_data_t;





void* ocurSecuencia1(void* args) {
      thread_data_t* data = args; 
      size_t thread_num = data->thread_num;
	  size_t lugares = data->cant_lugares;
      
      
      shared_data_t* shared_data = data->shared_data;
	  pthread_mutex_lock(&shared_data->mutex_array[thread_num]);
      size_t actual = shared_data->actual;
      
	   for(size_t i=actual;i<actual+lugares;++i){
		   
		    for(size_t i2=0;i2<26;++i2){
				
				if(shared_data->secuence1_array[i2].letra == DNA1[i]){
					++shared_data->secuence1_array[i2].frecuencia;
					
				}
				
				
		    }
			
	   }
       shared_data->actual+=data->cant_lugares;

	  
	  pthread_mutex_unlock(&shared_data->mutex_array[thread_num]);
	  
	  
    return NULL;
}

void* ocurSecuencia2(void* args) {
      thread_data_t* data = args; 
      size_t thread_num = data->thread_num;
	  size_t lugares = data->cant_lugares;
      
      
      shared_data_t* shared_data = data->shared_data;
	  pthread_mutex_lock(&shared_data->mutex_array[thread_num]);
      size_t actual = shared_data->actual;
      
	   for(size_t i=actual;i<actual+lugares;++i){
		   
		    for(size_t i2=0;i2<26;++i2){
				
				if(shared_data->secuence2_array[i2].letra == DNA2[i]){
					++shared_data->secuence2_array[i2].frecuencia;
					
				}
				
				
		    }
			
	   }
       shared_data->actual+=data->cant_lugares;
	  
	  pthread_mutex_unlock(&shared_data->mutex_array[thread_num]);
	  
	  
    return NULL;
}


void* commonLet(void* args) {
    thread_data_t* data = args; 
      size_t thread_num = data->thread_num;
	  shared_data_t* shared_data = data->shared_data;
	  
	  pthread_mutex_lock(&shared_data->mutex_array[thread_num]);
	   for(int i=0;i<DNA1_len;++i){
		   
           for(int i1=0;i1<DNA2_len;++i1){
                
               if(DNA1[i]==DNA2[i1]){
                    for(int i2=0;i2<26;++i2){
                        
                        if(shared_data->secuence1_array[i2].letra == DNA1[i]){
                            shared_data->common_array[i2].frecuencia=1;
                            
                        }  
                    }
                }   
           }	
	   }
	  
	  pthread_mutex_unlock(&shared_data->mutex_array[thread_num]);
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
    
	pthread_t* threads = malloc((size_t)(thread_count+1 * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    thread_data_t* thread_data = malloc((size_t)(thread_count * sizeof(thread_data_t)));
    pthread_mutex_t* mutex_list = malloc((size_t)(thread_count * sizeof(pthread_mutex_t)));
    
    dna_data_t* dna1_data_list = malloc((size_t)(26*sizeof(dna_data_t)));  
    dna_data_t* dna2_data_list = malloc((size_t)(26*sizeof(dna_data_t)));  
    dna_data_t* dna3_data_list = malloc((size_t)(26*sizeof(dna_data_t)));     
	


    for (size_t i = 0; i < thread_count; ++i) {
    pthread_mutex_init(&mutex_list[i], /*attr*/ NULL);
    } 

	//Para inicializar el vector de letras
    
    for(int i=0; i<26; i++){
        dna1_data_list[i].letra = 'a'+i;
        dna1_data_list[i].frecuencia = 0;
        dna2_data_list[i].letra = 'a'+i;
        dna2_data_list[i].frecuencia = 0;
        dna3_data_list[i].letra = 'a'+i;
        dna3_data_list[i].frecuencia = 0;
	}
    
	
   
    
	
    
    //Asignar el shared_data correctamente
    shared_data->mutex_array = mutex_list;
    shared_data->secuence1_array = dna1_data_list;  
    shared_data->secuence2_array = dna2_data_list;
    shared_data->common_array = dna3_data_list;
	shared_data->total = thread_count;     
    shared_data->actual = 0;
    

    //Creacion de los hilos
    //Calculo de limites de cada hilo
    int restantes = DNA1_len;
    int lugares  = 0;
    for (size_t i = 0; i < thread_count; ++i){
        thread_data[i].cant_lugares=0;
        
        if(restantes % thread_count==0){
         lugares = restantes/thread_count;
        }
        else{
           --restantes; 
           thread_data[i].cant_lugares=1;

        }
       
    }

    
    
    for (size_t i = 1; i < thread_count; ++i) {
     pthread_mutex_lock(&mutex_list[i]);
    } 

    for (size_t i = 0; i < thread_count; ++i){
        thread_data[i].cant_lugares+=lugares;
        thread_data[i].thread_num = i;
        thread_data[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, ocurSecuencia1, (void*)&thread_data[i]);
    }
    
    
    
    
    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
        pthread_mutex_unlock(&mutex_list[i+1]);
       
    }
	
    printf("Sequence 1. ocurrences: ");
	for (size_t i = 0; i < 26; ++i) {
        if(shared_data->secuence1_array[i].frecuencia!=0){
			printf("%c = %zu, ",shared_data->secuence1_array[i].letra,shared_data->secuence1_array[i].frecuencia);	 
		}	
       
    }
    printf("\nSequence 2. ocurrences: ");
    for (size_t i = 0; i < 26; ++i) {
        if(shared_data->secuence2_array[i].frecuencia!=0){
			printf("%c = %zu, ",shared_data->secuence2_array[i].letra,shared_data->secuence2_array[i].frecuencia);	 
		}	
       
    }


    printf("\nCommon letters: ");
    for (size_t i = 0; i < 26; ++i) {
        if(shared_data->common_array[i].frecuencia!=0){
			printf("%c, ",shared_data->common_array[i].letra);	 
		}	
    }
    printf("\n");
    
    
    
    free(threads);
    free(shared_data);
    free(thread_data);
    free(mutex_list);
    free(dna1_data_list);
    free(dna2_data_list);
    free(dna3_data_list);
    
    

    return 0;
}