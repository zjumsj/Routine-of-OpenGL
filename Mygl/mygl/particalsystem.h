#ifndef PARTICALSYSTEM_H
#define PARTICALSYSTEM_H

#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "particle.h"
#define PI 3.1415926
using namespace std;
class ParticalSystem
{
public:
	ParticalSystem();
	ParticalSystem(int _count, float _gravity) 
	{
		ptlCount = _count;
		gravity = _gravity;
	};
	void init();
	void simulate(float dt, Vector3D pos,float ratio);
	void aging(float dt, Vector3D pos,float ratio);
	void applyGravity();
	void kinematics(float dt);
	void checkBump(float x1, float x2, float y1, float y2);
	void render(float ratio);
	virtual ~ParticalSystem();
protected:
private:
	int ptlCount;
	float gravity;
	GLUquadricObj *mySphere;
	vector<Particle> particles;
};

#endif // PARTICALSYSTEM_H
