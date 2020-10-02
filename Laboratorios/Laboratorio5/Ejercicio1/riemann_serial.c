#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	float deltaDeX = 0;
	float a = 0;
	float b = 0;
	int n = 0;
	float areaTotal = 0;
	float xActual = 0;
	if(argc < 4){
		printf("Error, no ingresó los parametros solicitados.\n");
	}
	else{
		a = (float)strtoul(arg[1], NULL, 10);
		b = (float)strtoul(arg[2], NULL, 10);
		n = (int)strtoul(arg[3], NULL, 10);
		if(a > b || n < 1){
			printf("Error, parametro a > b o n < 1.\n");
		}
		else{
			xActual = a;
			deltaDeX = (b - a)/n;
			for(int i = 0; i < n; ++i){
				areaTotal += obtenerImagen(xActual) * deltaDeX;
				xActual += deltaDeX;
			}
			double duracion = walltime_elapsed(start);
			printf("El area total es %f, el tiempo de ejecucion fue de %f.\n", areaTotal, duracion);
		}
	}
	return 0;
}
