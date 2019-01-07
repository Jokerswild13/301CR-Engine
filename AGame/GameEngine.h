#pragma once
#include "GameObject.h"
#include <vector>
#include <enet/enet.h>
#include <vector>
extern "C" {
#include <../LUA/include/lua.h>
#include <../LUA/include/lauxlib.h>
#include <../LUA/include/lualib.h>
}
#include <../LUA/LuaBridge/LuaBridge.h>

class GameEngine 
{
private:
	
	static void updateGame(); ///<Function that updates the game state
	static int oldTimeSinceStart; ///<The old time since the start of the game (from previous frame) for delta tikme calculation.
	static int newTimeSinceStart; ///<The time since the start of the game  for delta tikme calculation.
	static unsigned int base; // Generate display list base. 	
	static GameObject* camerafollow; ///<Pointer to an object that the camera will follow.
public:
	static std::vector<GameObject*> gameobjects; ///<Vector of (pointers to) game objects: all the objects in the game.
	static void initEngine(int argc, char **argv, const char* windowTitle = "New Game", bool debug = false, int width = 500, int height = 500); ///<Initialises the engine, initilising glut and OpenGL.
	static void addGameObject(GameObject* gameobject, bool cameraFollow = false); ///<Adds a GameObject to the game.
	static void startEngine(); ///<Start the engine (and the main game loop).
	static void cleanup(); ///<Function to clean up and delete any pointers and anything else when the game exists to prevent memory leaks.
	static bool GameEngine::debugMode;
	static void displayFunc();
	static void reshapeFunc(int w, int h);
};