#include "GameEngine.h"
#include "SpaceShip.h"
#include "getbmp.h"


using namespace luabridge;


bool GameEngine::debugMode;
int GameEngine::oldTimeSinceStart; ///<The old time since the start of the game (from previous frame) for delta tikme calculation.
int GameEngine::newTimeSinceStart; ///<The time since the start of the game  for delta tikme calculation.
unsigned int GameEngine::base; // Generate display list base. 
static unsigned int MyTexture[10]; // also increase amount as textures grow
static unsigned int SunT[6];
int camera = 2;
lua_State* F;
float cameraDist;
float cameraHeight;



std::vector<GameObject*> GameEngine::gameobjects;
GameObject* GameEngine::camerafollow;
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.
void animate(int value);

///netwoking
ENetAddress address;
ENetHost* client;
ENetPeer* peer;
ENetEvent enetEvent;
ENetPacket* dataPacket;

//GameObject * ship = GameEngine::gameobjects[GameEngine::gameobjects.size() - 1];
struct Vector2 {
	float x;
	float y;
};
Vector2* newPosition = new Vector2;
void CameraFunc(unsigned char key, int x, int y)
{
	
	if (key == '2')
		camera = 2;
	if (key == '1')
		camera = 1;
	
	if (key == ' ') {
		if (isAnimate) isAnimate = 0;
		else
		{
			isAnimate = 1;
			animate(1);
		}

		dataPacket = enet_packet_create("packet", strlen("packet")+1 , ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(client, 0, dataPacket);
		
	}
	if (key == 27) {
		GameEngine::cleanup();
		exit(0);
		
	}
	glutPostRedisplay();
}


void animate(int value)
{
	if (isAnimate)
	{
		latAngle += 0.1;
		if (latAngle > 360.0) latAngle -= 360.0;
		longAngle += 0.1;
		if (longAngle > 360.0) longAngle -= 360.0;

		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 1);
	}
}


void GameEngine::initEngine(int argc, char **argv, const char* windowTitle, bool debug, int width, int height) {
	GameEngine::debugMode = debug;
	GameObject::debugMode = GameEngine::debugMode;
	///datadriven
	F = luaL_newstate();
	luaL_dofile(F, "camera.lua");
	luaL_openlibs(F);
	lua_pcall(F, 0, 0, 0);

	LuaRef t = getGlobal(F, "camera");
	LuaRef dist = t["distance"];
	LuaRef camHeight = t["yAm"];

	cameraDist = dist.cast<float>();
	cameraHeight = camHeight.cast<float>();

	///networking
	if (enet_initialize() != 0)
	{
		cout << "Enet failed to initialise!" << "\n\n";
	}

	/* Much of this will be familiar from the server code. What differs
	is the inclusion of an Peer. This is basically ENet's way of tracking
	what you're connected to. Since our Client will be initialising the
	connection, they need a peer to store information about the server they're
	connecting to. */

	

	client = enet_host_create(NULL, 1, 2, 0, 0);

	if (client == NULL)
	{
		cout << "Client failed to initialise!" << "\n\n";
	}

	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	/* Now we're trying to connect to the server. We include who we are,
	the address to establish the connection to, the number of channels, and a
	generic data variable we can use to say something to the server (currently 0). */

	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		cout << "No available peers for initializing an ENet connection.\n";
	}

	/* To make our lives a little easier, we declare this newPosition variable on
	the heap. It's where the data from our received packet is going to be stored,
	if it reaches us. */

	
	//Init glut.
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowTitle);

	glewExperimental = GL_TRUE;
	glewInit();

	glGenTextures(10, MyTexture);

	//Lambda function defined inside the glutDisplayFunc call to call all relevant drawscenes.
	glutDisplayFunc(displayFunc);
	glEnable(GL_DEPTH_TEST); // changes the depth so the track looks better
	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	// Enable light and set up 2 light sources (GL_LIGHT0 and GL_LIGHT1)
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);

	// We're setting up two light sources. One of them is located
	// on the left side of the model (x = -1.5f) and emits white light. The
	// second light source is located on the right side of the model (x = 1.5f)
	// emitting red light.

	// GL_LIGHT0: the white light emitting light source
	// Create light components for GL_LIGHT0
	float ambientLight0[] = {1.0f,  1.0f,  1.0f, 1.0f };
	float diffuseLight0[] = { 1.0f,  1.0f,  1.0f, 1.0f };
	float specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float position0[] = { -20.0f, 30.0f, 30.0f, 1.0f };
	
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	

	glLightfv(GL_LIGHT0, GL_POSITION, position0);	
	

	


	BitMapFile* image[10]; //amount of textures increase with more text
	image[0] = getbmp("./Textures/Texture0.bmp");
	//This is for texture 1
	glBindTexture(GL_TEXTURE_2D, MyTexture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	
	image[1] = getbmp("./Textures/Texture1.bmp");
	// This is for texture 2
	glBindTexture(GL_TEXTURE_2D, MyTexture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[2] = getbmp("./Textures/Texture2.bmp");
	// This is for texture 3
	glBindTexture(GL_TEXTURE_2D, MyTexture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[3] = getbmp("./Textures/Texture3.bmp");
	// This is for texture 4
	glBindTexture(GL_TEXTURE_2D, MyTexture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image[4] = getbmp("./Textures/Texture4.bmp");
	// This is for texture 5
	glBindTexture(GL_TEXTURE_2D, MyTexture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	
	glGenTextures(1, SunT);
	image[6] = getbmp("./Textures/SunT.bmp");
	// This is for texture 1
	glBindTexture(GL_TEXTURE_2D, SunT[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	//Lambda function defined inside glutReshapeFunc to do resize logic.
	glutReshapeFunc(reshapeFunc);

	//Lambda functions to link our code to glut's keydown andkeyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc(CameraFunc); 

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = false;
	});

	glutSpecialFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = true;
	});

	glutSpecialUpFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = false;
	});

	glutIdleFunc(updateGame);
}

 void GameEngine::displayFunc() {
	//Prepare for drawing all objects.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//If we have no object to follow just put the cam in a static position.
	if (camerafollow != NULL&&camera == 2) {
		//Hardcoded camera variables for the distance bewteen camera and object and y axis distance between camera and object.
			
		//float distance = 1;
		//float yAm = 60;


		gluLookAt(camerafollow->position.x - (camerafollow->heading.x * cameraDist), camerafollow->position.y - (camerafollow->heading.y * cameraDist) + cameraHeight, camerafollow->position.z - (camerafollow->heading.z * cameraDist),
			camerafollow->position.x, camerafollow->position.y, camerafollow->position.z,
			0.0, 1.0, 0.0);
	
	}
	if (camerafollow != NULL && camera == 1) {
		//Hardcoded camera variables for the distance bewteen camera and object and y axis distance between camera and object.

		float distance = 1;
		float yAm = 20;


		gluLookAt(camerafollow->position.x - (camerafollow->heading.x * distance), camerafollow->position.y - (camerafollow->heading.y * distance) + yAm, camerafollow->position.z - (camerafollow->heading.z * distance),
			camerafollow->position.x, camerafollow->position.y, camerafollow->position.z,
			0.0, 1.0, 0.0);

	}
	
	// enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	//Call drawscene for all gameobjects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->drawScene();
	}
	
	///skybox
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, -300, -300);
	glTexCoord2f(1.0, 0.0); glVertex3f(300, -300, -300);
	glTexCoord2f(1.0, 1.0); glVertex3f(300, 300, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, 300, -300);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[1]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(300, 300, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(-300, 300, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(-300, -300, 300);
	glTexCoord2f(0.0, 1.0); glVertex3f(300, -300, 300);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//bottom
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[2]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, -300, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(300, -300, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(300, -300, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, -300, -300);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//top side
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, 300, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(300, 300, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(300, 300, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, 300, -300);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[4]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, -300, -300);
	glTexCoord2f(1.0, 0.0); glVertex3f(-300, -300, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(-300, 300, 300);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, 300, -300);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTexture[4]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(300, -300, -300);
	glTexCoord2f(1.0, 0.0); glVertex3f(300, -300, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(300, 300, 300);
	glTexCoord2f(0.0, 1.0); glVertex3f(300, 300, -300);
	glEnd();
	
	/// skybox

	// Begin planet
	glPushMatrix();
	//glRotatef(longAngle, 0.0, 0.0, 1.0);
	GLUquadric * quad3 = gluNewQuadric();

	gluQuadricDrawStyle(quad3, GLU_FILL);
	gluQuadricNormals(quad3, GLU_SMOOTH);
	gluQuadricTexture(quad3, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SunT[6]);
	

	
	glTranslatef(-10.0, 0.0, -80.0);
	glRotatef(latAngle, 1.0, 0.0, 0.0);
	glTranslatef(10.0, 0.0, 80.0);

	glTranslatef(-10.0, -50.0, -45.0);

	glColor3f(1.0, 1.0, 1.0);
	gluSphere(quad3,60, 30, 30);
	// End planet.
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	gluQuadricDrawStyle(quad3, GLU_FILL);
	gluQuadricNormals(quad3, GLU_SMOOTH);
	gluQuadricTexture(quad3, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SunT[6]);
	glTranslatef(-10.0, -35.0, 280.0);
	glColor3f(1.1, 0.0, 0.0);
	gluSphere(quad3, 60, 30, 30);
	glPopMatrix();
	// End revolving ball.
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	

	glutSwapBuffers(); //Finish Draw Scene.
}


 void GameEngine::reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
};
 
 void GameEngine::addGameObject(GameObject* gameobject, bool cameraFollow) {
	 gameobjects.push_back(gameobject);
	 if (cameraFollow) {
		 camerafollow = gameobject;
	 }

	 base = glGenLists(1);
	 base = gameobject->setupDrawing(base);
	 gameobject->start();
 }

 void GameEngine::updateGame() {
	//Calculate delta time (in mili seconds).
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = newTimeSinceStart - oldTimeSinceStart;

	

	//If the last frame was rendered less than 1 ms ago, the detalaTime will be 0 ms. This causes problems in calculations, so sleep for 1ms to adjust.
	if (deltaTime == 0) {
		Sleep(1);
		newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newTimeSinceStart - oldTimeSinceStart;
	}


	//Run update for all game objects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}
	//Test collisions in a naive way for every pair of gameobjects (note calculations will explode when you increase the amount of gameobjects. Can you think of a better way?
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		for (std::vector<GameObject*>::size_type j = i + 1; j != gameobjects.size(); j++) {
			if (gameobjects[i]->collider != NULL) {
				if (gameobjects[i]->collider->collidesWith(gameobjects[j]->collider)) {
					gameobjects[i]->collides(gameobjects[j]->collider);
					gameobjects[j]->collides(gameobjects[i]->collider);
				}
			}
		}
	}
	while (enet_host_service(client, &enetEvent, 0) > 0)
	{
		switch (enetEvent.type) {

			/* Again, we're reacting based on the detected event type. In this case, it's
			ENET_EVENT_TYPE_RECEIVE, which means our client has recieved a packet from a
			peer (in this case, the server). After quickly outputting some debug text to
			console to confirm packet receipt, what happens next is the key part.

			Our packet has some intrinsic variables - its data (what it's storing) and its
			dataLength (how much data it's storing). In this case, since we KNOW the packet
			is a Vector2, we can use the memcpy function fairly easily. This is a basic C++
			function which copies a given amount of data from one location to another. In
			this case, it copies TO newPosition FROM data, and it copies an amount dataLength.

			Given what we know about pointer arithmetic, it should be obvious to us that we
			can make these packets more sophisticated. We can make huge packets which hold
			many different kinds of data. We simply include an enumerator at the beginning of
			each data segment, saying what data type it is, and either copy that much data over
			into a variable of that type, or include as the next element of a packet the amount
			of data this variable type needs to copy. This is particularly useful when it comes
			to Part 2 of the coursework, where 'level data' is likely very different to the
			'physics data' you'll have been transmitting for Part 1. */

		case ENET_EVENT_TYPE_RECEIVE:
			cout << "Packet received!\n";
			memcpy(newPosition, enetEvent.packet->data, enetEvent.packet->dataLength);
			cout << newPosition->x << "," << newPosition->y << "\n";
			//enemy.setPosition(sf::Vector2f(newPosition->x, newPosition->y));
			break;
		}
	}
	//Done so indicate that it's time to redraw.
	glutPostRedisplay();
 }
void GameEngine::startEngine() {
	//Explain main interaction with scene.
	//std::cout << "Press escape to exit." << std::endl;

	//Start game loop.
	
	glutMainLoop();
	
		
	
 }

void GameEngine::cleanup() {
	if (peer != NULL)
	{
		enet_peer_disconnect_now(peer, 0);
	}
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it) {
		delete *it;
	}
	gameobjects.clear();
 }