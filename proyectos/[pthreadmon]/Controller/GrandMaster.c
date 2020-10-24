#include "../Model/GrandMaster.h"

// ARREGLAR LO DEL TYPE DE LA MATRIZ DE WEAKNESSES STRENGTHS  ETC
// REVIZAR LA ACUMULACION DE ENERGIA PORQUE SIEMPRE HACE ATAQUE CARGADO /
// REVISAR LOS ATRIBUTOS DEL POKE
// REVIZAR EL FREE DE LA MEMORIA
// HACER INTERFAZ


/*Que hace cada mutex
mutex:
0. Para dejar entrar a pelear a los pokemones del jugador 0
1. Para dejar entrar a pelear a los pokemones del jugador 1
2. Chequear vida del pokemon en pelea del jugador 0
3. Chequear vida del pokemon en pelea del jugador 1
4. Chequear si hay cambio de pokemon del jugador 1
5. Chequear si hay cambio de pokemon del jugador 0
6. Dormir hasta que el oponente cambie de pokemon
7. Esperar a que los pokemones esten listos para pelear
*/

/*Que hace cada condVariable
condVariable:
0. hilo 0, pokemon 0 del jugador 0
1. hilo 1, pokemon 1 del jugador 0
2. hilo 2, pokemon 2 del jugador 0
3. hilo 3, pokemon 0 del jugador 1
4. hilo 4, pokemon 1 del jugador 1
5. hilo 5, pokemon 2 del jugador 1
6. GrandMaster
*/

/* 
 Metodo de dormir
*/

shared_data_t* shared_data_global;


void fixed_sleep(useconds_t duration){
	usleep(1000 * duration);

}

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);

	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;

	return elapsed;
}

void* fight (void* args) {
	thread_data_t* data = (thread_data_t*) args;
	int thread_num = data->threadNum;
	shared_data_t* shared_data = data->shared_data;
	
	float damage = 0.0;
	int esAtaqueCargado = 0;
	int moveType = 0;
	int cooldown = 0;

	int tengoVida = 1;
	int gane = 0;

	int oponente = 0;
	int yo = 1;
	int ultimoPokeOponente = 2;
	
	if(thread_num < 3){   //Para guardar cual es mi jugador oponente y quien soy yo
	 oponente = 1;
	 yo = 0;
	 ultimoPokeOponente = 5;
	}

	/*El pokemon se espera a ser invocado*/
	printf("Llego el thread %i\n", thread_num);
	
	pthread_barrier_wait(&shared_data->barreraDeInicio);
	
	if(thread_num == 0 || thread_num == 3){
		pthread_barrier_wait(&shared_data->barrera);
		pthread_cond_signal(&shared_data->condVariable[6]);
	}
	else{
		pthread_mutex_lock(&shared_data->mutex[yo]);
		pthread_cond_wait(&shared_data->condVariable[thread_num], &shared_data->mutex[yo]);
		pthread_mutex_unlock(&shared_data->mutex[yo]);
	}

	/*Iniciar tiempo de battalla del Pokemon*/
	walltime_start(data->tiempoStart);

	//mientras tenga vida
	while (tengoVida && !gane) {
		damage = 0.0;
		/*Fijarse si oponente tiene que hacer cambio de pokemon antes de seguir peleando*/
		pthread_mutex_lock(&shared_data->mutex[4 + oponente]);
		if(*shared_data->HP[shared_data->pokeDelJugActivo[oponente]] <= 0){
			if(shared_data->pokeDelJugActivo[oponente] == ultimoPokeOponente){
				printf("equipo %i gana\n", yo);
				gane = 1;
				pthread_mutex_unlock(&shared_data->mutex[4 + oponente]);
			}
			else{
				printf("%i: espera cambio de oponente\n", thread_num);
				pthread_mutex_lock(&shared_data->mutex[6]);
				pthread_mutex_unlock(&shared_data->mutex[4 + oponente]);
				pthread_cond_wait(&shared_data->condVariable[thread_num], &shared_data->mutex[6]);
				pthread_mutex_unlock(&shared_data->mutex[6]);
			}
		}
		else{
			pthread_mutex_unlock(&shared_data->mutex[4 + oponente]);
		}

		if(!gane){

			printf("energia de ataque cargado: %d, \n",data->pokemon->charged_move_data->energy);
		    printf("energia acumulada: %d, \n",*data->energia_acumulada );

			if (data->pokemon->charged_move_data->energy <= *data->energia_acumulada) {
				
				pthread_mutex_lock(&shared_data->mutex[2 + oponente]);
				printf("%i: usa ataque cargado\n", thread_num);
				shared_data->ataqueActual[yo] = 1;
				esAtaqueCargado = 1;
				moveType = data->pokemon->charged_move_data->typeId;
				cooldown = data->pokemon->charged_move_data->cooldown;
				*data->energia_acumulada -= data->pokemon->charged_move_data->energy;
				damage = data->pokemon->charged_move_data->power*2.3;      //power*bonus
				fixed_sleep(2000);
			}
			else{
				printf("%i: usa ataque rapido\n", thread_num);
				shared_data->ataqueActual[yo] = 2;
				esAtaqueCargado = 0;
				moveType = data->pokemon->fast_move_data->typeId;
				cooldown = data->pokemon->fast_move_data->cooldown;
				*data->energia_acumulada += data->pokemon->fast_move_data->energyGain;
				damage = data->pokemon->fast_move_data->power*2.3;
			}

			//weaknesses_matrix[aqui va el id tipo del pokemon del oponente][id del ataque rapido]
			if(weaknesses_matrix[shared_data->pokemones[shared_data->pokeDelJugActivo[oponente]]->typeId][moveType]){ 	//Si es muy efectivo
				damage *= 1.6;            							 	//power*bonus*effectiveness
			}
			else{
				if(resistances_matrix[shared_data->pokemones[shared_data->pokeDelJugActivo[oponente]]->typeId][moveType]){ //Si no es muy efectivo
					damage *= 0.625;          							//power*bonus*effectiveness
				}
				else{  													//Si no lo afecta
					damage *= 0.390625;       							//power*bonus*effectiveness
				}
			}
			
			damage+=1;                   							//(power*bonus*effectiveness)+1
			
			if(!esAtaqueCargado){
				pthread_mutex_lock(&shared_data->mutex[2 + oponente]);
			}

			/*Hacer daño al oponente*/
			*shared_data->HP[shared_data->pokeDelJugActivo[oponente]] -= damage;

			pthread_mutex_unlock(&shared_data->mutex[2 + oponente]);
			fixed_sleep(cooldown);


			/*Revisar mi vida*/
			pthread_mutex_lock(&shared_data->mutex[2 + yo]);
			shared_data->ataqueActual[oponente] = 0;
			if(data->pokemon->hp <= 0){
				tengoVida = 0;
			}
			//tengoVida = (data->pokemon->hp > 0);
			printf("%i: ataque cargado: %i, ataque rapido: %i, tipo del oponente: %i\n", thread_num, data->pokemon->charged_move_data->typeId, data->pokemon->fast_move_data->typeId, shared_data->pokemones[shared_data->pokeDelJugActivo[oponente]]->typeId);
			printf("%i: tiene %f\n", thread_num, data->pokemon->hp);
			printf("%i: oponente es: %i, tiene %f\n", thread_num, shared_data->pokeDelJugActivo[oponente] ,*shared_data->HP[shared_data->pokeDelJugActivo[oponente]]);
			printf("%i:  TengoVida = %i, gane = %i\n", thread_num, tengoVida, gane);
			pthread_mutex_unlock(&shared_data->mutex[2 + yo]);
			
		}
	}
	/*Sacar la duracion de combate del Pokemon*/
	*data->duracion = walltime_elapsed(data->tiempoStart);

	/*Cambio de pokemon*/
	if(!gane){
		pthread_mutex_lock(&shared_data->mutex[4 + yo]);
		if(shared_data->pokeDelJugActivo[yo] + 1 < yo * 3 + 3){
			++shared_data->pokeDelJugActivo[yo];
			printf("THREAD %i LLAMA A SIGUIENTE POKEMON................................\n", thread_num);
			pthread_cond_signal(&shared_data->condVariable[shared_data->pokeDelJugActivo[yo]]);
		}
		pthread_cond_signal(&shared_data->condVariable[shared_data->pokeDelJugActivo[oponente]]);
		pthread_mutex_unlock(&shared_data->mutex[4 + yo]);
	}
}



void iniciarBatalla(int* pokemonesID){
 
  

	//borrar
	for(int i = 0; i < 2; ++i){
		for(int j = 0; j < 3; ++j){
			printf("Jugador: %i, pokemon: %i\n", i, pokemonesID[i * 3 + j]);
		}
	}



	/*crear arreglos*/  
	pthread_mutex_t* mutex = malloc((size_t)(8 * sizeof(pthread_mutex_t)));
	pthread_cond_t* condVariable = malloc((size_t)(7 * sizeof(pthread_cond_t)));

	pthread_t* threads = malloc((size_t)(6 * 2 * sizeof(pthread_t)));
	
	shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
	thread_data_t* thread_data = malloc((size_t)(6 * sizeof(thread_data_t)));
	
	pokemon_data_t** losPokemones = malloc((size_t)(6 * sizeof(pokemon_data_t*)));

	walltime_t* tiemposStart = malloc((size_t)(7 * sizeof(walltime_t)));
	double* duraciones = malloc((size_t)(7 * sizeof(double)));
	int* energias_acumuladas = malloc((size_t)(6 * sizeof(int)));

	/*Crear los pokemones*/
	for(int i = 0; i < 6; ++i){
		losPokemones[i] = pokemon_create(pokemonesID[i]);
	}


	/*Asignacion de shared data*/
	shared_data->energias_acumuladas = energias_acumuladas;
    for(int i = 0; i < 6; ++i){
		shared_data->HP[i] = &losPokemones[i]->hp;
		shared_data->energias_acumuladas[i] = 0;
	}
	shared_data->pokemones = losPokemones;
	shared_data->mutex = mutex;
	shared_data->condVariable = condVariable;
	shared_data->pokeDelJugActivo[0] = 0;
	shared_data->pokeDelJugActivo[1] = 3;
	shared_data->ataqueActual[0] = 0;
	shared_data->ataqueActual[1] = 0;
	shared_data->tiemposStart = tiemposStart;
	shared_data->duraciones = duraciones;

	/*Variable global utilizada por la interfaz*/
	shared_data_global = shared_data; 

	/*Asignacion de thread_data*/
	for(int i = 0; i < 6; ++i){
		thread_data[i].threadNum = i;
		thread_data[i].pokemon = losPokemones[i];
		thread_data[i].shared_data = shared_data;
		thread_data[i].tiempoStart = &tiemposStart[i];
		thread_data[i].duracion = &duraciones[i];
		thread_data[i].energia_acumulada = &energias_acumuladas[i];
	}

	

	/*Inicializar mutex*/
	for(int i = 0; i < 8; ++i){
			pthread_mutex_init(&shared_data->mutex[i], NULL);
	}

	/*Inicializar variables condicionales*/
	for(int i = 0; i < 7; ++i){
		pthread_cond_init(&shared_data->condVariable[i], NULL);
	}

	/*Inicializar barrera*/
	pthread_barrier_init(&shared_data->barrera, NULL, 2);
	pthread_barrier_init(&shared_data->barreraDeInicio, NULL, 6);


	/*Crear los threads*/
	for(size_t i = 0; i < 6; ++i){
		pthread_create(&threads[i], NULL, fight, (void*)&thread_data[i]);
	}



	pthread_mutex_lock(&shared_data->mutex[7]);
	printf("GrandMaster espera a ser llamado\n");
	pthread_cond_wait(&shared_data->condVariable[6], &shared_data->mutex[7]);
	printf("GrandMaster despierta\n");
	/*Iniciar el tiempo de pelea*/
	walltime_start(&shared_data->tiemposStart[6]);



	/*join*/
	for (size_t i = 0; i < 6; ++i){ //el join no se si es necesarion
		pthread_join(threads[i], NULL);
	}

	/*finalizar tiempo de pelea*/
	shared_data->duraciones[6] = walltime_elapsed(&shared_data->tiemposStart[6]);

	pthread_cond_wait(&shared_data->condVariable[6], &shared_data->mutex[7]);
	pthread_mutex_unlock(&shared_data->mutex[7]);



	pthread_barrier_destroy(&shared_data->barrera);

	for(size_t i = 0; i < 8; ++i){
		pthread_mutex_destroy(&shared_data->mutex[i]);
	}

	for(size_t i = 0; i < 7; ++i){
		pthread_cond_destroy(&shared_data->condVariable[i]);
	}
	for(size_t i = 0; i < 6; ++i){
		pokemon_destroy(thread_data[i].pokemon);
	}


	pthread_barrier_destroy(&shared_data->barrera);
	pthread_barrier_destroy(&shared_data->barreraDeInicio);

	//liberar el vector de los pokemones

	//libberar el vector de las vidas, creo

    free(threads);
    free(mutex);
	free(condVariable);
	free(pokemonesID);
	free(tiemposStart);
	free(duraciones);
	free(energias_acumuladas);
	free(losPokemones);
	free(shared_data);
	free(thread_data);

	shared_data_global = 0;

}