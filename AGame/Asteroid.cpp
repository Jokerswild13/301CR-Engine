#include "Asteroid.h"
#include "CubeCollider.h"

static unsigned int AsteroidT[1];
Asteroid::Asteroid(glm::vec3 position, Colour colour) : GameObject(position) 
{
	this->colour = colour;
	Asteroid::collider = new CubeCollider(&this->position, 2, 2, 2);
}

Asteroid::~Asteroid()
{
	delete Asteroid::collider;
	Asteroid::collider = NULL;
}
unsigned int  Asteroid::setupDrawing(unsigned int listBase) {
	glGenTextures(1, AsteroidT);
	BitMapFile* image[1]; //amount of textures increase with more text
	image[0] = getbmp("./Textures/AsteroidT.bmp");
	// This is for texture 1
	glBindTexture(GL_TEXTURE_2D,AsteroidT[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->base = listBase;
	
	glPushMatrix();
	glNewList(this->base, GL_COMPILE);

	GLUquadric * quad = gluNewQuadric();

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, AsteroidT[0]);
	glRotatef(90, 0, 0, 1);
	     glColor3f(colour.r, colour.g, colour.b);
	     gluSphere(quad,1.5, 3, 3);
		/// glutSolidCube
	glPopMatrix();
	glEndList();
	return this->base +1;
}

void Asteroid::drawScene() {
	
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	float matAmbandDif1[] = { colour.r,colour.g,colour.b,1.0 };
	float specReflection3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float matShine1[] = { 40.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbandDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specReflection3);
	glCallList(this->base); // Draw Asteroid
	glPopMatrix(); // End draw Asteroid
				  
}

void Asteroid::start()
{
}
void Asteroid::update(int deltaTime)
{
}
void Asteroid::collides(Collider* other) {
	if (debugMode) {

		this->active = false;
	}
}

