/*
#define FREEGLUT_STATIC
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "mass.h"

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib" 
#else #define GL_Lib(name) name ".lib" 
#endif 
#endif

#pragma comment(lib, GL_Lib("freeglut_static"))

const int Window_Width = 600;
const int Window_Height = 600;
GLUquadricObj *mySphere;
Mass constVelBall;
Mass gravityBall;
Mass springBall;

void initGL(int width, int height)
{
	float ratio = (float)width / (float)height;
	mySphere = gluNewQuadric();
	// Our shading model--Gouraud (smooth).
	glShadeModel(GL_SMOOTH);
	// Set the clear color.
	glClearColor(0, 0, 0, 0);
	// Setup our viewport.
	glViewport(0, 0, width, height);
	//Change to the projection matrix and set our viewing volume.
	glEnable(GL_TEXTURE_2D);						// 启用纹理映射
	glMatrixMode(GL_PROJECTION);
	// 设置深度缓存
	glClearDepth(1.0);
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 所作深度测试的类型
	glDepthFunc(GL_LEQUAL);
	// 告诉系统对透视进行修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLoadIdentity();
	gluPerspective(60.0, ratio, 1.0, 100.0);
}
void drawMass(Mass m)
{
	Vector3D tmp = m.getPos();
	glPushMatrix();
	glTranslatef(tmp.x, tmp.y, tmp.z);
	gluSphere(mySphere, m.getSize(), 32, 16);
	glPopMatrix();
}
void initMasses()
{
	constVelBall = Mass(5.0, 3);
	constVelBall.init();
	constVelBall.setPos(Vector3D(5.0, 0.0, 0.0));
	constVelBall.setVel(Vector3D(3.0, 5.0, 0.0));

	gravityBall = Mass(5.0, 2);
	gravityBall.init();
	gravityBall.setPos(Vector3D(0.0, 8.0, 0.0));
	gravityBall.setVel(Vector3D(-1.0, 0.0, 0.0));
	gravityBall.setForce(Vector3D(0.0, -2.0, 0.0));

	springBall = Mass(5.0, 4);
	springBall.init();
	springBall.setPos(Vector3D(0.0, 0.0, 0.0));
	springBall.setVel(Vector3D(2.0, 0.0, 0.0));
}
void simulateCheck()
{
	Vector3D connectionPos = Vector3D(0.0f, 0.0f, 0.0f);
	constVelBall.simulate(0.1f);
	constVelBall.checkBnd(-20, 20, -20, 20);
	gravityBall.simulate(0.1f);
	gravityBall.checkBnd(-20, 20, -20, 20);

	springBall.simulate(0.1f);
	Vector3D springVector = springBall.getPos() - connectionPos;
	springBall.setForce(springVector*(-0.1f));
}
void renderGL()
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// We don't want to modify the projection matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Move down the z-axis.
	glTranslatef(0.0f, 0.0f, -35.0f);
	glColor3ub(0, 0, 255);										// Draw In Blue
	glBegin(GL_LINES);
	// Draw The Vertical Lines
	for (float x = -20; x <= 20; x += 2.0f)						// x += 1.0f Stands For 1 Meter Of Space In This Example
	{
		glVertex3f(x, 20, 0);
		glVertex3f(x, -20, 0);
	}
	// Draw The Horizontal Lines
	for (float y = -20; y <= 20; y += 2.0f)						// y += 1.0f Stands For 1 Meter Of Space In This Example
	{
		glVertex3f(20, y, 0);
		glVertex3f(-20, y, 0);
	}
	glEnd();
	glColor3ub(0, 255, 255);
	drawMass(constVelBall);
	glColor3ub(255, 0, 255);
	drawMass(gravityBall);
	glColor3ub(255, 255, 0);
	drawMass(springBall);
	glutSwapBuffers();
}
void Idle(void)
{
	simulateCheck();
	Sleep(10);
	renderGL();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition((1920 - Window_Width) / 2, (1080 - Window_Height) / 2 - 20);
	glutInitWindowSize(Window_Width, Window_Height);
	glutCreateWindow("physics");

	initGL(Window_Width, Window_Height);
	initMasses();
	
	glutDisplayFunc(renderGL);
	glutIdleFunc(Idle);
	glutMainLoop();
}
*/