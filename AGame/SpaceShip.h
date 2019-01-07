#pragma once
#include "GameObject.h"
#include<../glm/gtx/rotate_vector.hpp>
#include "getbmp.h"
#define TURNING_SPEED 70.0 //<turning speed in degrees per second
#define MOVE_SPEED 30.0 //<move speed in coordinate units per second


class SpaceShip:public GameObject
{
public:
	
	float Acceleration = 0.0;
	float Deacceleration = 0.0;
	SpaceShip(glm::vec3 pos);
	~SpaceShip();
	glm::vec3 startingHeading = heading = glm::vec3(0.0, 0.0, 1.0); // shows the starting position where the ship is facing
	float rotationAngle = 0.0; // rootating angle of the ship
	float pitchAngle = 0.0; // pitch angle
	unsigned int setupDrawing(unsigned int listBase);
	void start();
	void update(int deltaTime);
	void drawScene();
	void collides(Collider * other); ///<Override method that reacts to a collision.
};

