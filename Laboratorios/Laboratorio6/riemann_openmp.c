#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
typedef struct timespec walltime_t;

float obtenerImagen(float x){
	x *= x;
	++x;
	return x;
}

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);

	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;

	return elapsed;
}

int main(int argc, char* arg[]){
	walltime_t empezar;
	walltime_t* start = &empezar;
	walltime_start(start);
	long double deltaDeX = 0;
	long double a = 0;
	long double b = 0;
	size_t n = 0;
	long double areaTotal = 0;
	if(argc < 4){
		printf("Error, no ingresó los parametros solicitados.\n");
	}
	else{
		a = (long double)strtoul(arg[1], NULL, 10);
		b = (long double)strtoul(arg[2], NULL, 10);
		n = (long double)strtoul(arg[3], NULL, 10);
		if(a > b || n < 1){
			printf("Error, parametro a > b o n < 1.\n");
		}
		else{
			deltaDeX = (b - a)/n;
			
			#pragma parallel for default(none) shared(deltaDeX, n) reduction(+:areaTotal)
			for(size_t i = 0; i < n; ++i){
				long double xActual = a+i*deltaDeX;
				areaTotal += obtenerImagen(xActual) * deltaDeX;
				
			}
			double duracion = walltime_elapsed(start);
			printf("El area total es %Lf, el tiempo de ejecucion fue de %f.\n", areaTotal, duracion);
		}
	}
	return 0;
}
