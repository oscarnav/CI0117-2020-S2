#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct{
    //totals
    size_t thread_count;
    size_t hacker_thread_count;
    size_t serf_thread_count;
    //current passengers
    size_t hackers;
    size_t serfs;
    //shared structures
    sem_t hackerQueue;
    sem_t serfQueue;
    pthread_mutex_t mutex;
     pthread_mutex_t mutexBarrier;
	//pthread_barrier_t barrier;
	size_t cantidad;
	pthread_cond_t cond_var;
} shared_data_t;

typedef struct{
    int isCaptain;
    size_t thread_num;
    shared_data_t* shared_data;
} thread_data_t;

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

void* hacker(void* args) {

    random_sleep(1000, 2000);

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;

    pthread_mutex_lock(&shared_data->mutex);

    shared_data->hackers++;

    if (shared_data->hackers == 4) {
        for (size_t i = 0; i < 4; ++i) {
            sem_post(&shared_data->hackerQueue);
        }
        shared_data->hackers = 0;
        data->isCaptain = 1;

    } else if (shared_data->hackers == 2 && shared_data->serfs >= 2) {
        for (size_t i = 0; i < 2; ++i) {
            sem_post(&shared_data->hackerQueue);
            sem_post(&shared_data->serfQueue);
        }
        shared_data->hackers = 0;
        shared_data->serfs -= 2;
        data->isCaptain = 1;
    } else {
        pthread_mutex_unlock(&shared_data->mutex);
    }

    sem_wait(&shared_data->hackerQueue);

    //Board!
    	printf("------------------------Thread %zu (Hacker): On board!-------------------------\n", thread_num);

    //wait for 4 threads
    //pthread_barrier_wait(&shared_data->barrier);

    ++shared_data->cantidad;
	pthread_mutex_lock(&shared_data->mutexBarrier);
	if(shared_data->cantidad < 4){
		pthread_cond_wait(&shared_data->cond_var,&shared_data->mutexBarrier);
	}
	else{
		shared_data->cantidad=0;
        pthread_cond_broadcast(&shared_data->cond_var);
	}
	pthread_mutex_unlock(&shared_data->mutexBarrier);



    if (data->isCaptain) {
        //Row boat!
        random_sleep(1000, 2000);
        printf("\n===============================================================================\n");
        printf("-------------------------Thread %zu (Hacker): Row boat!!-----------------------\n", thread_num);
        printf("===============================================================================\n\n");
        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}


void* serf(void* args){
    random_sleep(1000, 2000);

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;

    pthread_mutex_lock(&shared_data->mutex);

    shared_data->serfs++;

    if (shared_data->serfs == 4) {
        for (size_t i = 0; i < 4; ++i) {
            sem_post(&shared_data->serfQueue);
        }
        shared_data->serfs = 0;
        data->isCaptain = 1;

    } else if (shared_data->serfs == 2 && shared_data->hackers >= 2) {
        for (size_t i = 0; i < 2; ++i) {
            sem_post(&shared_data->serfQueue);
            sem_post(&shared_data->hackerQueue);
        }
        shared_data->serfs = 0;
        shared_data->hackers -= 2;
        data->isCaptain = 1;
    } else {
        pthread_mutex_unlock(&shared_data->mutex);
    }

    sem_wait(&shared_data->serfQueue);

    //Board!
    	printf("-------------------------Thread %zu (Serf): On board!--- ----------------------\n", thread_num);

    //wait for 4 threads
    //pthread_barrier_wait(&shared_data->barrier);



    ++shared_data->cantidad;
	pthread_mutex_lock(&shared_data->mutexBarrier);
	if(shared_data->cantidad < 4){
		pthread_cond_wait(&shared_data->cond_var,&shared_data->mutexBarrier);
	}
	else{
		shared_data->cantidad=0;
        pthread_cond_broadcast(&shared_data->cond_var);
	}
	pthread_mutex_unlock(&shared_data->mutexBarrier);

    if (data->isCaptain) {
        //Row boat!
        random_sleep(1000, 2000);
        printf("\n==============================================================================\n");
        printf("-------------------------Thread %zu (Serf): Row boat!!-------------------------\n", thread_num);
        printf("===============================================================================\n\n");
        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}

int main(){

	//Variable declarations
	shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
	srand(time(NULL));
	shared_data->thread_count = ((rand()%10)+4);	
    shared_data->cantidad=0;
	//Initialization of shared resources
	sem_init(&shared_data->hackerQueue, 0, 0);
	sem_init(&shared_data->serfQueue, 0, 0);
	pthread_mutex_init(&shared_data->mutex, NULL);
	pthread_mutex_init(&shared_data->mutexBarrier, NULL);
	pthread_cond_init(&shared_data->cond_var, NULL);
	//pthread_barrier_init(&shared_data->barrier, NULL, 4);

	//Distribute thread types randomly
	shared_data->hacker_thread_count = rand()%shared_data->thread_count;
	shared_data->serf_thread_count = shared_data->thread_count - shared_data->hacker_thread_count;

	//Allocate memory for threads
	pthread_t* hacker_threads = malloc((size_t)(shared_data->hacker_thread_count * sizeof(pthread_t)));
	pthread_t* serf_threads = malloc((size_t)(shared_data->serf_thread_count * sizeof(pthread_t)));
	
	//Print randomly generated thread distribution
	printf("---------------------------River Crossing Problem----------------------------\n\n");
	printf("\t\t>>>Total number of hackers in queue: %zu  <<<\n", shared_data->hacker_thread_count);
	printf("\t\t>>>Total number of serfs in queue:   %zu  <<<\n\n", shared_data->serf_thread_count);
	printf("-----------------------Begin transportation procedure------------------------\n\n");
	
	//Create data lists for both thread types
	thread_data_t* thread_data_list_serf = malloc((size_t)(shared_data->serf_thread_count * sizeof(thread_data_t)));
	thread_data_t* thread_data_list_hacker = malloc((size_t)(shared_data->hacker_thread_count * sizeof(thread_data_t)));

	//Create serf threads
	for(size_t i = 0; i < shared_data->serf_thread_count; i++){
		thread_data_list_serf[i].isCaptain = 0;
		thread_data_list_serf[i].thread_num = i;
        	thread_data_list_serf[i].shared_data = shared_data;
		pthread_create(&serf_threads[i], NULL, serf, (void*)&thread_data_list_serf[i]);
	}
	
	//Create hacker threads
	for (size_t j = 0; j < shared_data->hacker_thread_count; j++){
		thread_data_list_hacker[j].isCaptain = 0;
                thread_data_list_hacker[j].thread_num = j;
                thread_data_list_hacker[j].shared_data = shared_data;
		pthread_create(&hacker_threads[j], NULL, hacker, (void*)&thread_data_list_hacker[j]);
	}

	//Join threads
	for(size_t i = 0; i < shared_data->thread_count; i++){
		pthread_join(serf_threads[i], NULL);
		pthread_join(hacker_threads[i], NULL);
	}
		
	//Memory cleanup
	free(hacker_threads);
	free(serf_threads);
	sem_destroy(&shared_data->hackerQueue);
	sem_destroy(&shared_data->serfQueue);
	pthread_mutex_destroy(&shared_data->mutex);
	pthread_mutex_destroy(&shared_data->mutexBarrier);
	pthread_cond_destroy(&shared_data->cond_var);
	return 0;
}
