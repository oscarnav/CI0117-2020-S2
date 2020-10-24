#ifndef GRAND_MASTER_H
#define GRANDMASTER_H

#include "pokemon.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct timespec walltime_t;

typedef struct{
    pthread_cond_t * condVariable;           //1 por thread/pokemon
    pthread_mutex_t* mutex;                  //1 mutex para empezar a pelear, otro para revizar la vida de uno;
    float* HP[6];                               //ver si funciona
    pthread_barrier_t barrera;                  //barrera que permite que los dos pokemones se empiezen a dañar al mismo tiempo y no uno antes que el otro
    pokemon_data_t** pokemones;
    walltime_t* tiemposStart;
    double* duraciones;
    int* energias_acumuladas;
    int pokeDelJugActivo[2];
    int ataqueActual[2];
} shared_data_t;

typedef struct{
    int threadNum;                              //numero del thread
    pokemon_data_t* pokemon;                  //tiene la info de a cual pokemon le toca controlar el thread
    shared_data_t* shared_data;
    walltime_t* tiempoStart;
    double* duracion;
    int* energia_acumulada;
} thread_data_t;

shared_data_t * shared_data_global;

void fixed_sleep(useconds_t duration);
void walltime_start(walltime_t* start);
double walltime_elapsed(const walltime_t* start);
void iniciarBatalla();


#endif