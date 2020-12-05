#include "../Model/World.h"
//Nota: hay que hacer srand(time(0)); en el main

World::World(){
	this->position = 0;
	this->colaEstatica = new queue<string>;
	this->colaVolatil = new queue<string>;
	for(int i = 0; i < 100; ++i){
		this->colaEstatica->push(MUNDO[i]);
		if(i < 10){
			this->colaVolatil->push("");
		}
	}
}

World::~World(){
	delete colaEstatica;
	delete colaVolatil;
}

int World::getPosition(){
	return position;
}

//para que mario avanze 1 casilla
string World::move(int &coins, int &imAlive, int &goomba, int &koopa,string hilera, int id, int &immune){
	string acontecimientos = "";
	string evento = "";
	queue<string>** cola[2];
	
	cola[0] = &colaEstatica;
	cola[1] = &colaVolatil;
	float random = 0;
	for(int i = 0; i < 2 && imAlive; ++i){
		string elementos = (*cola[i])->front();
		for(int j = 0; j < elementos.length() && imAlive; ++j){
			char elemento = elementos[j];
			random = float(rand()%100)/100;
			evento = "World pos. "+ to_string(position) + ": Mario #" + to_string(id);
			if(elemento == 's'){ //si me encuentro a una estrella(probabilidad de encontrarla = 1%)
				immune = 15;
				evento =  "\n*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\n\n*"+ evento + " has found a super star!! Mario got a star power, immune for " + to_string(immune) + " moves! ";
			}
			else if(elemento == 'g'){ //si me encuentro a un goomba
				evento += goombaEvent(random, imAlive, goomba, immune);
			}
			else if(elemento == 'c'){ //si me encuentro a una moneda
				evento += coinEvent(random, coins);
			}
			else if(elemento == 'k'){ //si me encuentro a un koopa
				evento += koopaEvent(random, imAlive, koopa, immune);
			}
			else{ //si me encuentro a un hoyo 
				evento += holeEvent(random, imAlive);
			}
			if(immune){ //si soy inmune imprimo cauntos movimientos me quedan para que se me acabe
				evento += "(immune for " + to_string(immune) + " moves).";
			}
			evento += " Coins: " + to_string(coins) + hilera;
			if(elemento == 's'){
				evento += "\n\n*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\n";
			}
			if(j+1 < elementos.length() && imAlive){
				evento += "\n";
			}
			acontecimientos += evento;
		}
		if(!imAlive){
			acontecimientos += "\nGame Over \n";
		}
		if(i == 0){
			(*cola[i])->pop();
			(*cola[i])->emplace(elementos);
		}
		else{
			(*cola[i])->pop();
		}
	}
	if(acontecimientos.length() == 0 && imAlive){ // si no hubo ningun acontecimiento
		acontecimientos += "World pos. " + to_string(position) + ": Mario #" + to_string(id) + " is walking.";
		if(immune){
			acontecimientos += " (immune for " + to_string(immune) + " moves).";
		}
		acontecimientos += " Coins: " + to_string(coins) + hilera;
	}
	
	if(position + 1 > 99){
		position = 0;
	}
	else{
		++position;
	}
	if(immune){
		--immune;
	}
	return acontecimientos;
}

string World::goombaEvent(float random, int &imAlive, int &goomba, int immune){
	string acontecimiento = "";
	if(random < 0.55){
		acontecimiento += " jumped over a little goomba! ";
	}
	else if(random < 0.95){
		acontecimiento += " jumped and killed a little goomba! ";
		++goomba;
	}
	else{
		if(immune){
			acontecimiento += " assassinated a little goomba with his star power!!! ";
		}
		else{
			acontecimiento += " didn't jump and was killed by a little goomba. ";
			imAlive = 0;
		}
	}
	return acontecimiento;
}

string World::coinEvent(float random, int &coins){
	string acontecimiento = "";
	if(random < 0.5){
		acontecimiento += " didn't grab a coin. ";
	}
	else{
		acontecimiento += " jumped and grabbed a coin!. ";
		++coins;
	}
	return acontecimiento;
}

string World::koopaEvent(float random, int &imAlive, int &koopa, int immune){
	string acontecimiento = "";
	if(random < 0.53){
		acontecimiento += " jumped over a koopa troopa! ";
	}
	else if(random < 0.9){
		acontecimiento += " jumped and killed a koopa troopa! ";
		++koopa;
	}
	else{
		if(immune){
			acontecimiento += " snapped a little koopa with his star power!!! ";
		}
		acontecimiento += " didn't jump and was killed by a koopa troopa. ";
		imAlive = 0;
	}
	return acontecimiento;
}

string World::holeEvent(float random, int &imAlive){
	string acontecimiento = "";
	if(random < 0.95){
		acontecimiento += " jumped over a hole! ";
	}
	else{
		acontecimiento += " didn't jump and died going into a hole. ";
		imAlive = 0;	
	}
	return acontecimiento;
}

//agregar enemigos que envian otros marios a este mundo
void World::addEnemies(int goomba, int koopa){
	int* enemigos[2];
	string celda = "";
	enemigos[0] = &goomba;
	enemigos[1] = &koopa;
	int super_star = rand()%100;
	if(!super_star){
		celda += "s";
	}
	for(int i = 0; i < 2; ++i){
		for(int j = 0; j < *enemigos[i]; ++j){
			if(i == 0){
				celda += "g";
			}
			else{
				celda += "k";
			}
		}
	}
	colaVolatil->emplace(celda);
}