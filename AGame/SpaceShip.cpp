#include "SpaceShip.h"
#include "CubeCollider.h"


static unsigned int SpaceshipT[10];
SpaceShip::SpaceShip(glm::vec3 position) : GameObject(position)
{
	
	
	SpaceShip::collider = new CubeCollider(&this->position, 3, 1.5, 3);
}


SpaceShip::~SpaceShip() {
	delete SpaceShip::collider;
	SpaceShip::collider = NULL;
}
unsigned int SpaceShip::setupDrawing(unsigned int listBase)
{

glGenTextures(10, SpaceshipT);
BitMapFile* image[2]; //amount of textures increase with more text
image[0] = getbmp("./Textures/SpaceshipT0.bmp");
// This is for texture 1
glBindTexture(GL_TEXTURE_2D, SpaceshipT[0]);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


GLUquadric * quad1 = gluNewQuadric();

gluQuadricDrawStyle(quad1, GLU_FILL);
gluQuadricNormals(quad1, GLU_SMOOTH);
gluQuadricTexture(quad1, GL_TRUE);
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glBindTexture(GL_TEXTURE_2D, SpaceshipT[0]);
//glEnable(GL_CULL_FACE);
glEnable(GL_TEXTURE_2D);

	this->base = listBase; // this creates our ship
	
	glNewList(this->base, GL_COMPILE);

	glPushMatrix(); // front of base front
	glBindTexture(GL_TEXTURE_2D, SpaceshipT[0]);
	glColor3f(1.3f, 0.2f, 0.2f);
	glVertex3f(-1.0f, 0.0f, 0.0f); glVertex3f(0.0f, -1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f); glEnd();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.0, 0.00, 0.4);
	gluDisk(quad1, 0, 1.2, 30, 6);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEndList();
	return this->base+1;
}
void SpaceShip::drawScene() {
	glBindTexture(GL_TEXTURE_2D, SpaceshipT[0]);
	
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	float AmbandDif1[] = {0.5,0.5,0.5,1.0};
	float specReflection2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float matShine1[] = { 40 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, AmbandDif1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection2);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine1 );
	//rotate openGL object
	glRotatef(pitchAngle, 1.0, 0.0, 0.0);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);
	glCallList(this->base); // Draw SpaceShip.
	glPopMatrix(); // End draw SpaceShip.
	//this->collider->Draw();
}

void SpaceShip::start()
{
}



void SpaceShip::update(int deltaTime)
{
	float moveStep = MOVE_SPEED * (deltaTime / 750.0); //movement speed in units per second * deltaTime in sec = moveStep
	float turningSpeed = TURNING_SPEED * (deltaTime / 750.0); //turning speed (degrees/sec) * deltaTime in sec = turning speed over delta time
	
	if (specialKeys[GLUT_KEY_DOWN]) {
		if (Deacceleration < 30.0f&& specialKeys[GLUT_KEY_DOWN] && Acceleration < 0.5f)
		{
			Deacceleration += 0.25;
		}

		this->position -= this->heading * (moveStep * (Deacceleration * deltaTime / 750));
	}
	    
		 if (specialKeys[GLUT_KEY_PAGE_UP]) {
			this->pitchAngle += turningSpeed; //in degrees not radians
		     }
		 if (specialKeys[GLUT_KEY_PAGE_DOWN]) {
			this->pitchAngle -= turningSpeed; //in degrees not radians
		     }
	     
		if (specialKeys[GLUT_KEY_LEFT]) {
			this->rotationAngle += turningSpeed; //in degrees not radians
		}
		if (specialKeys[GLUT_KEY_UP]) {
			if (Acceleration < 60.0f && specialKeys[GLUT_KEY_UP] && Deacceleration < 0.5f)
			{
				Acceleration += 0.4;
				score += 10;
				
			}
			this->position += this->heading * (moveStep * (Acceleration * deltaTime / 750));
		}
		if ((!specialKeys[GLUT_KEY_UP]) && Acceleration > 0.0f)
		{

			Acceleration -= 0.5;
			this->position += this->heading * (moveStep * (Acceleration * deltaTime / 750));
		}

		if ((!specialKeys[GLUT_KEY_DOWN]) && Deacceleration > 0.0f)
		{

			Deacceleration -= 0.5;
			this->position -= this->heading *(moveStep * (Deacceleration * deltaTime / 750));
		}
		if (specialKeys[GLUT_KEY_RIGHT]) {
			this->rotationAngle -= turningSpeed; //in degrees not radians
		}

		//rotate the heading
		//In the drawscene we will need to rotate by the full rotation since the start
		//Therfore we start heard from the original startingheading, so that we can use the same variables for the rotations
		this->heading = glm::rotate(this->startingHeading, glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));
		this->heading = glm::rotate(this->heading, glm::radians(pitchAngle), glm::vec3(0.0, 0.0, 1.0));
	}


void SpaceShip::collides(Collider* other)
{
	if (debugMode) {
		std::cout << "SpaceShip collides!" << std::endl;
		Deacceleration = 0;
		Deacceleration = Acceleration;
		Acceleration = 0;


	}
}