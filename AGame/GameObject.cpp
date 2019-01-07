#include "GameObject.h"

bool GameObject::debugMode;
std::map <int, bool> GameObject::specialKeys;
std::map <char, bool> GameObject::keys;
GameObject::GameObject(glm::vec3 position)
{
	this->position = position;
}

GameObject::~GameObject()
{
}

void GameObject::drawScene()
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glCallList(this->base);
	glPopMatrix();
}
void GameObject::collides(Collider* other) {
}
