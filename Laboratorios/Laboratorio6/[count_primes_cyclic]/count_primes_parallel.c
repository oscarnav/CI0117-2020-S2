#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



int main(int argc, char* arg[]) {


size_t contador = 0;
if (argc >= 1) {
        
		size_t ultimo = (size_t)strtoul(arg[4], NULL, 10);
		if (ultimo <= 0){
			ultimo = 1;
		}
		
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }
	#pragma omp parallel for reduction(+: contador)
	for (int i = 2; i < ultimo; i++) {        
         if(esPrimo(i)){
			 ++contador;
		 }

    }
	
	
	
	
}


int esPrimo(size_t number){
	
	if(number < 2){
		return false;
	}
	if(number == 2){
		return true;
	}
	if(number % 2 == 0){
		return false;
	}
	
	for(size_t i = 3, last = (size_t)(double)sqrt(number); i <= last; i += 2){
		if(number%i == 0){
			return false;
		}
	}
	return true;
}	
