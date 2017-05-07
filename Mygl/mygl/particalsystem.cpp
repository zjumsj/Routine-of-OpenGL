/*****************************************************************************
Copyright: 2012, ustc All rights reserved.
contact:k283228391@126.com
File name: particalsystem.Cpp
Description:Partical in opengl.
Author:Silang Quan
Version: 1.0
Date: 2012.12.22
 *****************************************************************************/

#include "particalsystem.h"

ParticalSystem::ParticalSystem()
{
    //ctor
}

ParticalSystem::~ParticalSystem()
{
    //dtor
}

void ParticalSystem::init()
{
    int i;
    srand(unsigned(time(0)));
    Color colors[10]={
		{0.3f,	0.7f,	1.0f,	1.0f},
		{0.5f,	0.75f,	1.0f,	1.0f},
		{0.3f,	0.75f,	0.9f,	1.0f},
		{0.4f,	0.7f,	1.0f,	1.0f},
		{0.5f,	0.65f,	1.0f,	1.0f},
		{0.5f,	0.4f,	1.0f,	1.0f},
		{0.2f,	0.45f,	1.0f,	1.0f},
		{0.5f,	0.45f,	0.9f,	1.0f},
		{0.4f,	0.5f,	1.0f,	1.0f},
		{0.5f,	0.45f,	1.0f,	1.0f}
	};
	for(i=0;i<ptlCount;i++)
    {
		Particle tmp = {
			Vector3D(0,0,0),
			Vector3D(0,0,0),
			Vector3D(0,0,0),
			colors[rand()%10],
			0.0f,
			0.05*(rand()%2)+0.5f,
			0.3f
		};
        particles.push_back(tmp);
    }
    mySphere=gluNewQuadric();
}
void ParticalSystem::simulate(float dt, Vector3D pos,float ratio)
{
    aging(dt,pos,ratio);
    applyGravity();
    kinematics(dt);
    //checkBump(-15,15,-15,15);
}
void ParticalSystem::aging(float dt, Vector3D pos, float ratio)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        iter->age+=dt;
        if(iter->age>iter->life)
        {
            iter->position=pos;
			iter->age=0.1*(rand()%10);
			iter->velocity=Vector3D((float)(rand()%10-5.0f)*ratio,(float)(rand()%10-5.0f)*ratio,-3.0*ratio);
        }
    }
}
void ParticalSystem::applyGravity()
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
            iter->acceleration=Vector3D(0,0,gravity);
}

void ParticalSystem::kinematics(float dt)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        iter->position = iter->position+iter->velocity*dt;
        iter->velocity = iter->velocity+iter->acceleration*dt;
    }
}
void ParticalSystem::checkBump(float x1,float x2,float y1,float y2)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
        {
            if (iter->position.x - iter->size < x1 || iter->position.x +iter->size > x2) iter->velocity.x = -iter->velocity.x;
            if (iter->position.y - iter->size < y1 || iter->position.y + iter->size > y2) iter->velocity.y = -iter->velocity.y;
        }
}
void ParticalSystem::render(float ratio)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        float alpha = 1 - iter->age / iter->life;
        Vector3D tmp=iter->position;
        glColor4f(iter->color.r,iter->color.g,iter->color.b,alpha);
        glPushMatrix();
        glTranslatef(tmp.x,tmp.y,tmp.z);
        gluSphere(mySphere,iter->size*0.2, 8, 8);
        glPopMatrix();
    }
}
