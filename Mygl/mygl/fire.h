#ifndef FIRE_H
#define FIRE_H
#include "particalsystem.h"

extern void fire_init(ParticalSystem *fire, int fire_num, float ratio);
extern void fire_sim(ParticalSystem *fire, int fire_num, Vector3D *pos, float ratio);//in idle
extern void fire_render(ParticalSystem *fire, int fire_num, float ratio);//in display func

#endif