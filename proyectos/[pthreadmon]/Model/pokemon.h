#ifndef POKEMON_H
#define POKEMON_H
#include <pthread.h>
#include "../Model/mapper.h"



typedef struct fast_move
{

int id;
int cooldown;
char* moveName;
int energy;
int energyGain;
int power;
int typeId;

} fast_move_t;

typedef struct charged_move
{
int id;
int cooldown;
char* moveName;
int energy;
int energyGain;
int power;
int typeId;
} charged_move_t;

typedef struct pokemon
{
   int id;
   char* species_name;
   int fastMoveId;
   int chargedMoveId;
   int typeId;
   char* typeName;
   float hp;
   fast_move_t* fast_move_data;
   charged_move_t* charged_move_data;
   //aqui van los atributos del pokemon
   //dos punteros a structs que tienen la info del fastmove y  el charged move

} pokemon_data_t;




pokemon_data_t* pokemon_create(int  id);
void pokemon_destroy(pokemon_data_t* pokemon);

#endif
