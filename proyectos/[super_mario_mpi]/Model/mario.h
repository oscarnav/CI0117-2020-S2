#ifndef MARIO_H
#define MARIO_H

#include "../Model/World.h"

class Mario{
private:
int* location;
int coins;
int isAlive;
int koopas;
int goombas;
int immune;
char strategy;
World* world;

   

public:
Mario();
Mario(char strategy);
~Mario();

void setStrategy(char strategy,int n);
int getMonedas();
int getLife();
int getObjective(int* posiblesVictimas, int* monedas, int* victimas, int id, int vivos); //vivos es para recorrer posibles victimas
char getStrategy();
int getGoombas();
int getKoopas();
int getAttackers(int* victimas,int id, int num_processes);
string generarInfo(int oponentes_vivos, int* posibles_victimas, int* victimas, int my_id, int vivos, int victima);

string Avanzar( string hilera, int id);
void getAttacked(int,int);
void resetObstacles();

//hay que estructurarlo
//int choseAction(actionList);

};
#endif
