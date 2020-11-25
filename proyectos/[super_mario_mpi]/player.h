#ifndef PLAYER_H
#define PLAYER_H


typedef struct player
{
	Mario* mario;
	World* world
	int isActive
   
} player_t;

player_t* player_create();
void player_destroy(player_t* player);

#endif