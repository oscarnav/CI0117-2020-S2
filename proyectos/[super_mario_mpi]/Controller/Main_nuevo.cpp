#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <math.h>

static string ESTRATEGIA[] = {"RANDOM","LESS_COINS", "MORE_COINS", "ATTACKER"}
static string CHAR_ESTRATEGIA = {'R','L','M','A'};

//se puede poner dentro de mario
string generarInfo(int oponentes_vivos, int* posibles_victimas, int* victimas, int my_id, int vivos, int victima, char strategy){
    string info_eventos = " | atacking #" + victima + " | being attacked by";
    for(int i = 0; i < oponentes_vivos; ++i){
        if(victimas[posibles_victimas[i]] == my_id){
            info_eventos += " #" + posibles_victimas[i];
        }
    }
    if(info_eventos == "| being attacked by"){
        info_eventos += " 0";
    }
    
    info_eventos += " | attack strategy: ";
    for(int i = 0; i < 4; ++i){
        if(strategy == CHAR_ESTRATEGIA[i]){
            info_eventos += ESTRATEGIA[i];
        }
    }
    
    info_eventos += " | Total playig: " + vivos + \n;
    return info_eventos;
}



int main(int argc, char* argv[]) {
    Mario* mario = new Mario();
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
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
    
    
    
    int* monedas = int[num_processes];  //Arreglo de monedas de todos los marios
    int* vidas = int[num_processes];    //Arreglo de vidas de todos los marios
    int* victimas = int[num_processes]; //Arreglo de a quien atacan

    if (my_id == 0) {
        while(vivos != 1){
            
             
             
         }
    } 
    else{
        //si soy el mario que pidio el usuario
        if(argv[1] == my_id){
            Mario*mario = new Mario(argv[2]);
        }
        //si no
        else{
            Mario* mario = new Mario();    
        }
        
        strategy = mario->getStrategy();
        im_alive = mario->getLife();
        //hacer allreduce de vivos
        //hacer todos los gathers menos el de victimas
        
        while(im_alive && vivos > 1){
            
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
            if(strategy = 'R' && !vidas[victima] || strategy != 'R'){
                victima = mario->getObjective(posibles_victimas, vivos -1, monedas, victimas, my_id);
            }
            
            //hacer el gather de victimas
            
            //hacer un metodo por aparte para esto
            eventos_de_mario = mario->guess_the_attackers(vivos-1, posibles_victimas, victimas, my_id, vivos, victima, strategy);
            eventos_de_mario = mario->avanzar(); //pasarle parametros para construir  | atacking #3 | being attacked by #5 | attack strategy: RANDOM | Total playing: 4
            goombas = mario->getGoombas();
            koopas = mario->getKoopas();
            mario->resetObstacles();
            
            //hacer el send goombas a mi victima
            //hacer el send koopas a mi victima
            //hacer el receive goombas a mi victima
            //hacer el receive koopas a mi victima
            
            im_alive = mario->getLife();
            //hacer allreduce de vivos
            //hacer allgather de coins y vidas y espectador
            
            delete[] posibles_victimas;
        }
      
         while(vivos > 1){
            //el formato de arriba de reduces y allgathers
         }
        

    }
    //MPI_Bcast(&ultimo, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //MPI_Allgather(&my_id, 1, MPI_INT, recv_buffer, 1, MPI_INT,MPI_COMM_WORLD);
    
    
    delete mario;
    MPI_Finalize();
    return 0;
}