/*
 * http://www.cnblogs.com/liangliangh/
 --------------------------------------------------------------------------------*/

#ifndef _GL_INC_
#define _GL_INC_


#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glfw3dll.lib")

#pragma comment (lib, "glu32.lib")    // link OpenGL Utility lib
#pragma comment (lib, "opengl32.lib") // link Microsoft OpenGL lib

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#endif // #ifndef _GL_INC_