#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float obtenerImagen(float x){
	x *= x;
	++x;
	return x;
}

int main(argc, args[]){
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
			for(int i = 0; i < n, ++i){
				areaTotal += obtenerImagen(xActual) * deltaDeX;
				xActual += deltadeX;
			}
			printf("El area total es %f.\n", areaTotal);
		}
	}
	return 0;
}
