
//#include "Collider.h" //gameobjects can have a collider
#include "GameObject.h"
#include "getbmp.h"
using namespace std;

struct Colour
{
	float r, g, b;
};

class Asteroid: public GameObject

{
private:
	
	Colour colour;
public:
	unsigned int setupDrawing(unsigned int listBase);
	~Asteroid();
	void start();
	void update(int deltaTime);
	Asteroid(glm::vec3 pos, Colour col = { 0,0,1 });
	void collides(Collider * other);///<Overridden collides method.
	void drawScene();
};