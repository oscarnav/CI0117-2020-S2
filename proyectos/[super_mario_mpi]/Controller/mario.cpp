#include <stdlib.h>
#include "../Model/mario.h"

const string ESTRATEGIA[] = {"RANDOM","LESS_COINS", "MORE_COINS", "ATTACKER"};
const char CHAR_ESTRATEGIA[] = {'R','L','M','A'};
//Constructor
Mario::Mario() {
	isAlive=1;
	coins = 0;
	koopas = 0;
	goombas = 0;
	strategy = CHAR_ESTRATEGIA[rand()%4];
	world = new World();
	//location
}

Mario::Mario(char strategy) {
	isAlive=1;
	coins = 0;
	koopas = 0;
	goombas = 0;
	this->strategy = strategy;
	world = new World();
	this->strategy = CHAR_ESTRATEGIA[rand()%4];
	//location
}

//Destructor
Mario::~Mario() {
	delete world;
}

void Mario::setStrategy(char strategy,int n){
	this->strategy = strategy;
	if(n!=-1){
		this->strategy = CHAR_ESTRATEGIA[n];
	}
}

int Mario::getMonedas(){
	return coins;
}

int Mario::getObjective(int* posiblesVictimas, int* monedas, int* victimas, int id, int vivos){
	int objective;
	if(strategy == 'R'){  //random
		objective = posiblesVictimas[rand()%vivos];
	}
	else if(strategy == 'L'){  // less coins
		int menor = monedas[posiblesVictimas[0]];
		objective = posiblesVictimas[0];
		for(int i= 1; i<vivos ; ++i){
			if(monedas[posiblesVictimas[i]] < menor){
				menor = monedas[posiblesVictimas[i]];
				objective = posiblesVictimas[i];
			}
			
		}

	}
	else if(strategy == 'M'){  // most coins
		int mayor = monedas[posiblesVictimas[0]];
		objective = posiblesVictimas[0];
		for(int i= 1; i<vivos ; ++i){
			if(monedas[posiblesVictimas[i]] > mayor){
				mayor = monedas[posiblesVictimas[i]];
				objective = posiblesVictimas[i];
			}

		}

	}
	else{				//atacker
		objective = posiblesVictimas[rand()%vivos];
		for(int i= 0; i<vivos ; ++i){
			if(victimas[posiblesVictimas[i]] == id){
				objective = posiblesVictimas[i];
			}

		}


	}
	return objective;
}

int Mario::getAttackers(int* victimas, int id, int num_processes){
	int attackers = 0;
	for(int i = 0 ; i<num_processes ; ++i){
		if(victimas[i] == id){
			++attackers;
		}
	
	}
	
	return attackers;
}


char Mario::getStrategy(){
	return strategy;
}

int Mario::getLife(){
	return isAlive;
}

int Mario::getGoombas(){
	return goombas;
}

int Mario::getKoopas(){
	return koopas;
}

//Para invocar metodo hace que mario se mueva y vea los siguientes elementos
string Mario::Avanzar( string hilera, int id){
	string next = world->move(coins,isAlive,goombas,koopas ,hilera, id, immune);
	return next;
}

//genera el string informativo: a quien ataco, quienes me atacan, mi tipo de estrategia, total de marios vivos 
string Mario::generarInfo(int oponentes_vivos, int* posibles_victimas, int* victimas, int my_id, int vivos, int victima){
	string info_eventos = " | atacking #" + to_string(victima) + " | being attacked by";
    int atacantes = 0;
	for(int i = 0; i < oponentes_vivos; ++i){
        if(victimas[posibles_victimas[i]] == my_id){
            info_eventos += " #" + to_string(posibles_victimas[i]);
			++atacantes;
        }
    }
    if(atacantes == 0){
        info_eventos += " 0 Marios";
    }
    
    info_eventos += " | attack strategy: ";
	info_eventos += this->strategy;
    
    info_eventos += " | Total playing: " + to_string(vivos);
    return info_eventos;
}

//Para invocar metodo que agrega elementos que me envian otros marios
void Mario::getAttacked(int goomba, int koopa){
	world->addEnemies(goomba,koopa);
}

void Mario::resetObstacles(){
	koopas = 0;
	goombas = 0;
}


