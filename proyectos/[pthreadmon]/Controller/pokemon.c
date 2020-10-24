#include <stdio.h>
#include <stdlib.h>
#include "../Model/pokemon.h"

pokemon_data_t* pokemon_create(int  id){
    pokemon_data_t* pokemon = calloc(1, sizeof(pokemon_data_t));
    pokemon->fast_move_data = calloc(1, sizeof(fast_move_t));
    pokemon->charged_move_data = calloc(1, sizeof(charged_move_t));


    initialize_data();
  
    pokemon->id = id;
    pokemon->species_name = get_pokemon_species_name(id);//DE la clase mapper
    pokemon->fastMoveId =  get_pokemon_fast_move_id(id);
    pokemon->chargedMoveId =  get_pokemon_charged_move_id(id);
    pokemon->typeId =  get_pokemon_type_id(id);
    pokemon->typeName = get_type_name(pokemon->typeId);
    pokemon->hp = 1500.0;
    
    //Para setear la informacion en el struct del fast move
    pokemon->fast_move_data->id = pokemon->fastMoveId;                                   //todos estos metodos de  get son pertenecientes a la clase mapper.c
    pokemon->fast_move_data->cooldown = get_move_cooldown(pokemon->fastMoveId);
    pokemon->fast_move_data->moveName = get_move_name(pokemon->fastMoveId);
    pokemon->fast_move_data->energy = get_move_energy(pokemon->fastMoveId);
    pokemon->fast_move_data->energyGain = get_move_energy_gain(pokemon->fastMoveId);
    pokemon->fast_move_data->power = get_move_power(pokemon->fastMoveId);
    pokemon->fast_move_data->typeId = get_move_type_id(pokemon->fastMoveId);
    
    //Para setear la informacion en el struct del charged move
    pokemon->charged_move_data->id = pokemon->chargedMoveId;                             //todos estos metodos de  get son pertenecientes a la clase mapper.c
    pokemon->charged_move_data->cooldown = get_move_cooldown(pokemon->chargedMoveId);
    pokemon->charged_move_data->moveName = get_move_name(pokemon->chargedMoveId);
    pokemon->charged_move_data->energy = get_move_energy(pokemon->chargedMoveId);
    pokemon->charged_move_data->energyGain = get_move_energy_gain(pokemon->chargedMoveId);
    pokemon->charged_move_data->power = get_move_power(pokemon->chargedMoveId);
    pokemon->charged_move_data->typeId = get_move_type_id(pokemon->chargedMoveId);
    
 
    
    return pokemon;
    
}


void pokemon_destroy(pokemon_data_t* pokemon){
    free(pokemon->fast_move_data);
    free(pokemon->charged_move_data);
    free(pokemon);


}


