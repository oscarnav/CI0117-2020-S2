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


int calcularPrimos(int my_id, int ultimo, int num_processes){
    int total = ultimo-1;
    int cociente = total/(num_processes-1);
    int residuo = total%(num_processes-1);
    int cantPorAnalizar = cociente;
    int empezar = cociente*(my_id-1);
    int contador = 0;
    
    if(residuo >= my_id){
        ++cantPorAnalizar;
        empezar+= my_id-1;
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
    
    return contador;
}



int main(int argc, char* argv[]) {


    int ultimo = 0;
    
    if (argc >= 2) {
		ultimo = (size_t)strtoul(argv[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    double t1, t2;
    int my_id;
    int num_processes;
    int message_sent = 0;
    int message_received = 0;
    int total_de_primos = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    if(my_id != 0){
        message_sent = calcularPrimos(my_id, ultimo, num_processes);
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, 0 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }
    else{
        t1 = MPI_Wtime();
        for(int i = 0; i < num_processes-1; ++i){
            MPI_Recv(&message_received, 1 /* count*/, MPI_INT,MPI_ANY_SOURCE /*source*/, MPI_ANY_TAG /*message id*/, MPI_COMM_WORLD, &status);
            total_de_primos += message_received;
        }
        t2 = MPI_Wtime();
        printf("%i primes found in range [%i,%i] in %fs with %i processes\n", total_de_primos, 2, ultimo, t2-t1, num_processes);
    }
    
  

    MPI_Finalize();
	return 0;
}
