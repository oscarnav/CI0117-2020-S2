#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <math.h>
#include <limits>
#include <ios>
#include "../Model/mario.h"

int pedirMario(int espectador,int* vidas,int vivos,int num_processes){
    if(!vidas[espectador] && vivos > 1){
                int esValido = 0;
                cout << "Mario vivos: " << endl;
                while(!esValido){
                    for(int i = 0; i<num_processes;++i){
                        if(vidas[i]){
                            printf("Mario #%i\n",i);
                        }
                    }
                    cout << "Ingrese siguiente Mario a espectar" << endl;
                    //cin >> espectador;
                    try{
                        cin >> espectador;
                        if(cin.fail()){
                          throw 1;
                        }
                        else{
                        esValido = (0 < espectador && espectador < num_processes && vidas[espectador] == 1 );
                        }
                    }
                    catch(int error){
                        esValido = 0;
                        espectador = 0;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    }
                }
    }

    return espectador;
}




int main(int argc, char* argv[]) {
    Mario* mario = new Mario();
    MPI_Status status;
    int num_processes;
    int my_id;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    srand((unsigned) time(NULL) * my_id * 1000);
    int proceso_mostrar;//proceso elegido para ser mostrado en pantalla
    int vivos = 0; //numero de procesos vivos, se va a obtener con un all reduce 
    int espectador = 0;
    
    int im_alive = 0; //booleano de si yo mario estoy vivo o no
    int coins = 0;      // monedas que tiene ese mario  puede que no lo necesite
    int victima = 0;    //mario que voy a atacar
    int atacante = -1;   //mario que me ataca
    char strategy;      //estrategia del mario
    string eventos_de_mario = "";
    int goombas = 0;
    int koopas = 0;
    int cantidad_de_atacantes = 0;
    
    
    int* monedas = new int[num_processes]();  //Arreglo de monedas de todos los marios inicializado en 0
    int* vidas = new int[num_processes]();    //Arreglo de vidas de todos los marios inicializado en 0
    int* victimas = new int[num_processes](); //Arreglo de a quien atacan inicializado en 0
    vivos = num_processes-1;
    for(int i = 1; i < num_processes; ++i){
        vidas[i] = 1;
    }

    if (my_id == 0) {
        espectador = (int)strtoul(argv[1], NULL, 10);
        
        int ganador = 0;
        while(vivos > 1){
            //Se hace gather de todos los datos de los Marios
            MPI_Allgather(&victima, 1, MPI_INT, victimas, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Allreduce(&im_alive, &vivos, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            MPI_Allgather(&im_alive, 1, MPI_INT, vidas, 1, MPI_INT,MPI_COMM_WORLD);
            MPI_Allgather(&coins, 1, MPI_INT, monedas, 1, MPI_INT,MPI_COMM_WORLD);
            
            //En caso de que el Mario a espectar muera se  le pide otro al usuario
            espectador = pedirMario(espectador,vidas,vivos,num_processes);
            //Se les comunica a los demas procesos el Mario a espectar
            MPI_Bcast(&espectador, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        if(num_processes > 1){//Hay menos de dos Marios vivos
            
            if(vivos == 1){
                MPI_Recv (&ganador, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
                printf("Mario #%i gana!!!\n",ganador);
            }
            else{
                printf("No hubo ganador\n");
            }
            
        }
    } 
    
    //entonces no soy el proceso 0
    else{
        //si soy el mario que pidio el usuario
        espectador = (int)strtoul(argv[1], NULL, 10);
        if(espectador == my_id){
            mario->setStrategy((char)*argv[2],-1);
        }
        else{//Si no soy el Mario que pidio el usuario
            mario->setStrategy('_',rand()%4);
            
        }
        strategy = mario->getStrategy();
        im_alive = mario->getLife();
        coins = mario->getMonedas();
        
        
        while(im_alive && vivos > 1){
            int koopas_recibidos = 0;
            int goombas_recibidos = 0;
            int cantidad_de_atacantes = 0;
            /*crear un arreglo con todos los posibles marios a los que se les puede atacar*/
            int* posibles_victimas = new int[vivos-1];
            int j = 0;
            for(int i = 0; i < num_processes; ++i){
                if(vidas[i] && i != my_id){
                    posibles_victimas[j] = i;
                    ++j;
                }
            }

            /*Setear a quien ataco*/
            if(strategy == 'R' && !vidas[victima] || strategy != 'R'){
                victima = mario->getObjective(posibles_victimas,monedas, victimas,my_id,vivos -1);
            }
    
            //hacer el gather de victimas
            MPI_Allgather(&victima, 1, MPI_INT, victimas, 1, MPI_INT, MPI_COMM_WORLD);
            
            //hacer un metodo por aparte para esto
            
            eventos_de_mario = mario->generarInfo(vivos-1, posibles_victimas, victimas, my_id, vivos, victima);
            eventos_de_mario = mario->Avanzar(eventos_de_mario, my_id); //pasarle parametros para construir  | atacking #3 | being attacked by #5 | attack strategy: RANDOM | Total playing:
            
            //enviarle lo que pasó con
            if(my_id == espectador){
                cout << eventos_de_mario << endl;
            }
            
            goombas = mario->getGoombas();
            koopas = mario->getKoopas();
            mario->resetObstacles();
            
            //hacer el send goombas a mi victima
            MPI_Send (&goombas, 1, MPI_INT, victima, 123, MPI_COMM_WORLD);
            //hacer el send koopas a mi victima
            MPI_Send (&koopas, 1, MPI_INT, victima, 124, MPI_COMM_WORLD);
           
           cantidad_de_atacantes = mario->getAttackers(victimas, my_id, num_processes);
            //hacer el receive goombas a mi victima
            //hacer el receive koopas a mi victima
            for(int i = 0; i < cantidad_de_atacantes; ++i){
                MPI_Recv (&goombas, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
                MPI_Recv (&koopas, 1, MPI_INT, MPI_ANY_SOURCE, 124, MPI_COMM_WORLD, &status);
                goombas_recibidos += goombas;
                koopas_recibidos += koopas;
            }
   
            mario->getAttacked(goombas_recibidos, koopas_recibidos);
            
            im_alive = mario->getLife();
            
            //Se hace gather de todos los datos de los Marios
            MPI_Allreduce(&im_alive, &vivos, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            MPI_Allgather(&im_alive, 1, MPI_INT, vidas, 1, MPI_INT,MPI_COMM_WORLD);
            MPI_Allgather(&coins, 1, MPI_INT, monedas, 1, MPI_INT,MPI_COMM_WORLD);
            //Los Marios reciben el Mario a mostrar
            MPI_Bcast(&espectador, 1, MPI_INT, 0, MPI_COMM_WORLD);
            
            delete[] posibles_victimas;
        }
        
        victima = -1;
        
        if(im_alive){
            //El ganador le comunica al proceso 0 que es el ganador
             MPI_Send (&my_id, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
        }
        else{
            while(vivos > 1){
                //Marios muertos hacen llamados para mantener el flujo de la ejecucion
                MPI_Allgather(&victima, 1, MPI_INT, victimas, 1, MPI_INT, MPI_COMM_WORLD);
                MPI_Allreduce(&im_alive, &vivos, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                MPI_Allgather(&im_alive, 1, MPI_INT, vidas, 1, MPI_INT,MPI_COMM_WORLD);
                MPI_Allgather(&coins, 1, MPI_INT, monedas, 1, MPI_INT,MPI_COMM_WORLD);
                MPI_Bcast(&espectador, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
        }
        

    }
    //printf();
    delete[] monedas;
    delete[] vidas;
    delete[] victimas;
    delete mario;
    MPI_Finalize();
    return 0;
}