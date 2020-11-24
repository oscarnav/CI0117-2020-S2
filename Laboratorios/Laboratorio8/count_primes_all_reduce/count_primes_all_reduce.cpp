#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
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


void calcularPrimos(int my_id, int ultimo, int num_processes ){
    double t1, t2;
    
    t1 = MPI_Wtime();
    int total = ultimo-1;
    int cociente = total/num_processes;
    int residuo = total%num_processes;
    int cantPorAnalizar = cociente;
    int empezar = cociente*my_id;
    int contador = 0;
    int resultado = 0;
    int ultimoAnalizado = 0;
    
    if(residuo > my_id){
        ++cantPorAnalizar;
        empezar+= my_id;
    }
    else{
        empezar += residuo;
    }
    empezar += 2;
    
    //(25-1)/5 = 4
    //(25-1)%5 = 4
    //proceso1 = 5   2+ 0     empezar = 0
    //proceso2 = 5   2+ 5     empezar = 
    //proceso3 = 5   2+ 10    empezar = 
    //proceso4 = 5   2+ 15    empezar = 
    //proceso5 = 4   2+ 20    empezar = 

    
    for(int i = empezar; i < empezar+cantPorAnalizar; ++i){
        if(esPrimo(i)){
            //printf("%i es primo", i);
            ++contador;
        }
    }
     
    t2 = MPI_Wtime();
    MPI_Allreduce(&contador, &resultado, 1, MPI_INT, MPI_SUM,MPI_COMM_WORLD);

    ultimoAnalizado = empezar+cantPorAnalizar-1;
    printf("process %i: %i out of %i primes found in range [%i, %i] in %fs with %i processes\n", my_id, contador, resultado, empezar, ultimoAnalizado,t2-t1, num_processes);

}



int main(int argc, char* argv[]) {
    int ultimo = 0;
    int my_id;
    int num_processes;
    int total_de_primos = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);


    if (my_id==0) {
		printf("¿Hasta cual numero desea averguar la cantidad de primos?\n");
        std::cin >> ultimo; 
            if(!std::cin){
                printf("numero no valido\n");
            }

    } 
    MPI_Bcast(&ultimo, 1, MPI_INT, 0, MPI_COMM_WORLD);

    calcularPrimos(my_id, ultimo, num_processes);
    
    MPI_Finalize();
    return 0;
}