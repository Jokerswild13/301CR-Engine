#include "CubeCollider.h"

CubeCollider::CubeCollider(glm::vec3 * centre, float cubeWidth, float cubeHeight, float cubeLength) {
	this->colliderCentre = centre;
	this->width = cubeWidth;
	this->height = cubeHeight;
	this->length = cubeLength;
}


CubeCollider::~CubeCollider() {
}

float CubeCollider::minX() {
	return this->colliderCentre->x - this->width / 2;
}

float CubeCollider::maxX() {
	return this->colliderCentre->x + this->width / 2;
}

float CubeCollider::minY() {
	return this->colliderCentre->y - this->height / 2;
}

float CubeCollider::maxY() {
	return this->colliderCentre->y + this->height / 2;
}

float CubeCollider::minZ() {
	return this->colliderCentre->z - this->length / 2;
}

float CubeCollider::maxZ() {
	return this->colliderCentre->z + this->length / 2;
}

bool CubeCollider::collidesWith(Collider* other) {
	if (other == NULL) {
		return false;
	}

	bool overlapX = other->minX() <= this->maxX() && other->maxX() >= this->minX();
	bool overlapY = other->minY() <= this->maxY() && other->maxY() >= this->minY();
	bool overlapZ = other->minZ() <= this->maxZ() && other->maxZ() >= this->minZ();
	return overlapX && overlapY && overlapZ;
	//	return overlapX && overlapZ;
}

//Draw cube ith vertices so that we can show it in debug mode.
void CubeCollider::Draw() {
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(this->minX(), this->minY(), this->minZ());
	glVertex3f(this->maxX(), this->minY(), this->minZ());

	glVertex3f(this->maxX(), this->minY(), this->minZ());
	glVertex3f(this->maxX(), this->minY(), this->maxZ());

	glVertex3f(this->maxX(), this->minY(), this->minZ());
	glVertex3f(this->maxX(), this->maxY(), this->minZ());

	glVertex3f(this->maxX(), this->maxY(), this->minZ());
	glVertex3f(this->maxX(), this->maxY(), this->maxZ());

	glVertex3f(this->maxX(), this->maxY(), this->maxZ());
	glVertex3f(this->maxX(), this->minY(), this->maxZ());

	glVertex3f(this->maxX(), this->minY(), this->maxZ());
	glVertex3f(this->minX(), this->minY(), this->maxZ());

	glVertex3f(this->minX(), this->minY(), this->maxZ());
	glVertex3f(this->minX(), this->minY(), this->minZ());

	glVertex3f(this->minX(), this->minY(), this->minZ());
	glVertex3f(this->minX(), this->maxY(), this->minZ());

	glVertex3f(this->minX(), this->maxY(), this->minZ());
	glVertex3f(this->maxX(), this->maxY(), this->minZ());

	glVertex3f(this->minX(), this->maxY(), this->minZ());
	glVertex3f(this->minX(), this->maxY(), this->maxZ());

	glVertex3f(this->minX(), this->minY(), this->maxZ());
	glVertex3f(this->minX(), this->maxY(), this->maxZ());

	glVertex3f(this->minX(), this->maxY(), this->maxZ());
	glVertex3f(this->maxX(), this->maxY(), this->maxZ());

	glEnd();
	glPopMatrix();
}