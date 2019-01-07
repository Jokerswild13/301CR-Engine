
#include "GameEngine.h"
#include "SpaceShip.h"
#include "Asteroid.h"




	int main(int argc, char **argv) {

		//Initialise the engine.
		GameEngine::initEngine(argc, argv, "SpaceShip tutorial", true, 1240, 720);
		
	//Adding an asteroid.
	int v = 6; // smaller square
	for (int p = 0;p < 26; p++) //first inside line (right)
	{
		GameEngine::addGameObject(new Asteroid(glm::vec3(7.0, 0.0, v-14.5), { 0.75f, 0.25f, 0.65f }));
		v +=9;
	} 
	
	//Adding an asteroid with different colour.
	float s = -8.5; // larger square
	int m = 10;
	for (int p = 0; p < 26; p++) // first outside line (right)
	{
		GameEngine::addGameObject(new Asteroid(glm::vec3(-m, 0.0,s), { 0.75f, 0.25f, 0.65f }));
		s += 9; // 7 is the distance between them
	}
	//1st obstacle
	GameEngine::addGameObject(new Asteroid(glm::vec3(1, 0.0, 28), { 0.75f, 0.25f, 0.65f }));
	GameEngine::addGameObject(new Asteroid(glm::vec3(3, 0.0, 28), { 0.75f, 0.25f, 0.65f }));
	GameEngine::addGameObject(new Asteroid(glm::vec3(5, 0.0, 28), { 0.75f, 0.25f, 0.65f }));
	//2nd obstacle
	GameEngine::addGameObject(new Asteroid(glm::vec3(-4, 0.0, 73), { 0.75f, 0.25f, 0.65f }));
	GameEngine::addGameObject(new Asteroid(glm::vec3(-6, 0.0, 73), { 0.75f, 0.25f, 0.65f }));
	GameEngine::addGameObject(new Asteroid(glm::vec3(-8, 0.0, 73), { 0.75f, 0.25f, 0.65f }));
	
	//Add a movable ship.
	
	GameEngine::addGameObject(new SpaceShip(glm::vec3(-2.0, 0.0, -5.0)), true);

	
	//Start the game engine.
	GameEngine::startEngine();

	return 0;
}
	
	