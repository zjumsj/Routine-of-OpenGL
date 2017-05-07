/*
 * http://www.cnblogs.com/liangliangh/
 --------------------------------------------------------------------------------*/

#ifndef _BT_INC_
#define _BT_INC_

#ifdef _DEBUG
	#pragma comment (lib, "LinearMath32d.lib")
	#pragma comment (lib, "BulletCollision32d.lib")
	#pragma comment (lib, "BulletDynamics32d.lib")
	#pragma comment (lib, "BulletSoftBody32d.lib")
#else
	#pragma comment (lib, "LinearMath32.lib")
	#pragma comment (lib, "BulletCollision32.lib")
	#pragma comment (lib, "BulletDynamics32.lib")
	#pragma comment (lib, "BulletSoftBody32.lib")
#endif

#include"btBulletDynamicsCommon.h"


#endif // #ifndef _BT_INC_