#include "fire.h"
void fire_init(ParticalSystem *fire, int fire_num, float ratio)
{
	int i, particle_num = 100;
	float accleration = 50.0;
	i = sizeof(ParticalSystem);
	for (i = 0;i < fire_num;i++)
	{
		(*(fire + i)) = ParticalSystem(particle_num, accleration*ratio);
		(*(fire + i)).init();
	}
}
void fire_sim(ParticalSystem *fire, int fire_num,Vector3D *pos, float ratio)
{
	float refresh_speed = 0.02;
	int i, sleep_time = 10;
	for (i = 0;i < fire_num;i++)
		(*(fire + i)).simulate(refresh_speed, *(pos+i), ratio);
	Sleep(sleep_time);
}
void fire_render(ParticalSystem *fire, int fire_num, float ratio)
{
	int i;
	for (i = 0;i < fire_num;i++)
		(*(fire + i)).render(ratio);
}