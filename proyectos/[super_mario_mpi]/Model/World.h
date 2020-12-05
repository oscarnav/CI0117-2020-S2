#ifndef WORLD
#define WORLD
#include <string>
#include <queue>
#include <cstdlib>
#include <iostream>
using namespace std;

static string MUNDO[] = {"","","","","","","","","","c","","","gcc","","","","","","","","","g","","","","","g","g","","","","","","","","h","h","","","",
"g","g","","","h","h","","","c","g","g","","","k","c","c","","c","gg","","","","","gg","","ggc","gc","","","","","","","","","","",
"h","h","","","","","","","","c","","gg","","","","","","","","","","",""}; //arreglo de 100

class World{
	private:
		queue<string>* colaEstatica;
		queue<string>* colaVolatil;
		int position;
	public:
		World();
		~World();
		int getPosition();		//puede que no lo necesite
		string move(int &coins, int &imAlive, int &goomba, int &koopa, string hilera, int id);
		string goombaEvent(float random, int &imAlive, int &goomba);
		string coinEvent(float random, int &coins);
		string koopaEvent(float random, int &imAlive, int &koopa);
		string holeEvent(float random, int &imAlive);
		void addEnemies(int goomba, int koopa);
};
#endif