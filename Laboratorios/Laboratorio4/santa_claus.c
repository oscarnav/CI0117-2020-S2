#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<semaphore.h>


typedef struct {

    size_t total;
	size_t cantTerminados;
	
	size_t reindeer;
	size_t elves;

	pthread_cond_t ready;

	sem_t reindeerSem;
	sem_t santaSem;
	pthread_barrier_t barrera;
	pthread_mutex_t reindeerMutex;
	pthread_mutex_t elfTex;
	pthread_mutex_t mutex;
	pthread_mutex_t mutexGeneral;
} shared_data_t;

 
size_t cantidadDeElves(size_t minElves, size_t maxElves){//revisar que sea div por 3
    size_t cantElves = minElves;
    size_t range = maxElves - minElves;
    if ( range > 0 ){
		cantElves += rand() % range;
	}
	if(cantElves % 3){
		cantElves += 3 - (cantElves % 3);
	}
	return cantElves;
}

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){

	useconds_t duration = min_milliseconds;
    useconds_t range = max_milliseconds - min_milliseconds;
    if (range > 0)
		duration += rand() % range;
    usleep( 1000 * duration );

}

/*Metodos de Santa*/
void prepareSleigh(){
	
}

void helpElves(){
	
}
void* santaTask(void* args){
    shared_data_t* data = (shared_data_t*) args;
	int continuar = 1;
	while(continuar){
		pthread_mutex_lock(&data->mutexGeneral);
		if(data->total == data->cantTerminados){
			printf("Santa terminó su labor.\n");
			continuar = 0;
			pthread_mutex_unlock(&data->mutexGeneral);
		}
		else{
			pthread_mutex_unlock(&data->mutexGeneral);
			printf("Esperando\n");
			sem_wait(&data->santaSem);
			if(data->reindeer >= 9){
				printf("SANTA AYUDA A RENOS\n");
				prepareSleigh();
				for(int i=0; i < data->reindeer;++i){
					sem_post(&data->reindeerSem);
				}
				data->reindeer -= 9;
			}
			else if(data->elves == 3){
				printf("SANTA AYUDA A DUENDES\n");
				pthread_barrier_wait(&data->barrera);
				helpElves();
			}
		}
	}
}



/*Metodos de Reindeer*/
void getHitched(){
	
}
void* reindeerTask(void* args){
    shared_data_t* data = (shared_data_t*) args;
	printf("Reno en vacaciones\n");
	random_sleep(15000,35000);
	pthread_mutex_lock(&data->reindeerMutex);
	++data->reindeer;
	printf("Reno espera a sus compañeros\n");
    if(data->reindeer >= 9){
        sem_post(&data->santaSem);
    }
	pthread_mutex_unlock(&data->reindeerMutex);
    sem_wait(&data->reindeerSem);
    getHitched();
	pthread_mutex_lock(&data->mutexGeneral);
	printf("Reno se va\n");
	++data->cantTerminados;
	printf("Total de ayudados es: %zu\n", data->cantTerminados);
	pthread_mutex_unlock(&data->mutexGeneral);
}



/*Metodos de elf*/
void getHelp(){

}
void* elfTask(void* args){
    shared_data_t* data = (shared_data_t*) args;
	printf("Duende trabaja\n");
	random_sleep(5000,50000);
    pthread_mutex_lock(&data->elfTex);
    pthread_mutex_lock(&data->mutex);
	printf("Duende pasa a sala de espera\n");
    data->elves += 1;
    	if(data->elves == 3){
        	sem_post(&data->santaSem);
    	}
    	else{	
			pthread_mutex_unlock(&data->elfTex);
    	}
    
    pthread_mutex_unlock(&data->mutex);
	getHelp(data);
	pthread_barrier_wait(&data->barrera);
	pthread_mutex_lock(&data->mutexGeneral);
	printf("Duende es ayudado por Santa\n");
    pthread_mutex_lock(&data->mutex);
    data->elves -=1;
    	if(data->elves == 0){
      	 	pthread_mutex_unlock(&data->elfTex);
    	}
    pthread_mutex_unlock(&data->mutex); 
	++data->cantTerminados;
	printf("Total de ayudados es: %zu\n", data->cantTerminados);
	if(data->cantTerminados == data->total){
		sem_post(&data->santaSem);
	}
	pthread_mutex_unlock(&data->mutexGeneral);
}



int main(){
	
	
	size_t cantidadReindeer = 9;
	size_t cantidadElves = cantidadDeElves(6,30);
	printf("La cantidad de duendes es: %zu\n", cantidadElves);
	/*Referencia a los threads*/
	pthread_t santa; 
    pthread_t* threadsReindeer = malloc((size_t)(cantidadReindeer * sizeof(pthread_t)));
	pthread_t* threadsElves = malloc((size_t)(cantidadElves * sizeof(pthread_t)));
	
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

	/*Asignacion de shared_data*/
	
	shared_data->reindeer = 0;
	shared_data->elves = 0;
    shared_data->total = cantidadReindeer + cantidadElves;
	shared_data->cantTerminados = 0;

	/*Creacion de semaforos, mutex*/

	sem_init(&shared_data->santaSem, 0, 0);
	sem_init(&shared_data->reindeerSem, 0, 0);
	
	pthread_mutex_init(&shared_data->mutexGeneral, NULL);
	pthread_mutex_init(&shared_data->reindeerMutex, NULL);
    pthread_mutex_init(&shared_data->mutex, /*attr*/ NULL);
    pthread_mutex_init(&shared_data->elfTex, NULL);

	pthread_barrier_init(&shared_data->barrera, NULL, 4);

	pthread_cond_init(&shared_data->ready, NULL);

	/*crear el thread de Santa*/
	pthread_create(&santa, NULL, santaTask, (void*)shared_data);
	
	/*crear los threads de reindeer*/
    for (size_t i = 0; i < cantidadReindeer; ++i){
        pthread_create(&threadsReindeer[i], NULL, reindeerTask, (void*)shared_data);
    }	
	
	/*crear los threads de elves*/
    for (size_t i = 0; i < cantidadElves; ++i) {
        pthread_create(&threadsElves[i], NULL, elfTask, (void*)shared_data);
    }

	/*Esperar threads*/
	pthread_join(santa, NULL);

    for (size_t i = 0; i < cantidadReindeer; ++i) {
        pthread_join(threadsReindeer[i], NULL);
    }
    for (size_t i = 0; i < cantidadElves; ++i) {
        pthread_join(threadsElves[i], NULL);
    }

	/*Liberar memoria*/
    sem_destroy(&shared_data->reindeerSem);
	sem_destroy(&shared_data->santaSem);

    pthread_mutex_destroy(&shared_data->elfTex);
	pthread_mutex_destroy(&shared_data->mutex);
	pthread_mutex_destroy(&shared_data->reindeerMutex);
	pthread_mutex_destroy(&shared_data->mutexGeneral);

	pthread_barrier_destroy(&shared_data->barrera);
	pthread_cond_destroy(&shared_data->ready);


    free(threadsReindeer);
	free(threadsElves);
    free(shared_data);

    return 0;
}





