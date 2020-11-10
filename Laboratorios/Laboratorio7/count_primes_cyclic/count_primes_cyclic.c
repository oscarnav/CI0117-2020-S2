#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>


int esPrimo(size_t number){
	
	if(number < 2){
		return 0;
	}
	if(number == 2){
		return 1;
	}
	if(number % 2 == 0){
		return 0;
	}
	
	for(size_t i = 3, last = (size_t)(double)sqrt(number); i <= last; i += 2){
		if(number%i == 0){
			return 0;
		}
	}
	return 1;
}	


int main(int argc, char* arg[]) {
size_t ultimo = 0;

size_t contador = 0;
	if (argc >= 2) {
		ultimo = (size_t)strtoul(arg[1], NULL, 10);
	
		
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }
 
  size_t num_threads = 10;

 //export OMP_SCHEDULE = "STATIC,1"
	#pragma omp parallel for num_threads(num_threads) reduction(+: contador) schedule(runtime)
	for (size_t i = 2; i <= ultimo; i++) {        
		 if(esPrimo(i)){
			
			 ++contador;
		 }

    }
	printf("numero de primos: %zu\n", contador);
	
	
	
	
}



