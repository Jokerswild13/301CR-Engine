#pragma once
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <../GL/glew.h>
#  include <../GL/freeglut.h>
#  include <../GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 

#include "../glm/glm.hpp"
#include <map>
#include <iostream>
#include "Collider.h" //gameobjects can have a collider
using namespace std;
class GameObject
{
protected:
	unsigned int base;
public:
	int score;
	static bool GameObject::debugMode;
	static std::map <int, bool> GameObject::specialKeys;
	static std::map <char, bool> GameObject::keys;
	bool active = true;
	Collider* collider = NULL;
	
	glm::vec3 heading = glm::vec3(0.0, 0.0, 1.0);  //The direction it is facing
	glm::vec3 position;//The position
	GameObject(glm::vec3 position);
	~GameObject();


	virtual unsigned int setupDrawing(unsigned int listBase) =0;
	virtual void drawScene(); ///Function to perform this object's part of the redraw work. The default version in GameObject will put the object in the right place and redraw it. This can be overridden.
							  /* Every GameObject must implement the following pure virtual function or otherwise cause a compile error.*/
	virtual void start() = 0; ///<Pure virtual start function, run once when the engine starts.  Must override in inherited classes.
	virtual void update(int deltaTime) = 0; ///<Pure virtual update function, run periodicity to update the game state. Must override in inherited classes.
	virtual void collides(Collider* other); ///<Virtual function to notify this game object that a collision has happened can be overridden (optional).
};

