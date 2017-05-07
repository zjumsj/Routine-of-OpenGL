
#include "gl_inc.h"
#include "ft_inc.h"
#include "il_inc.h"
#include <vector>
using namespace std;


class Mo;
class manage;
class object;

//全局变量
class Mo{
public: 
	static int State;//1 表示游戏状态 0 表示调试状态
	static POINT mousepoint;
	static int flash;
	static int flag_d;
	static int flag_d1;
	static float angle_d;
	static int appear;
	static float p_m;

public:
	static void Mode(void);
	static void level(float &x, float &y, float &z);
};
/*
  1 | 2 | 3
  4 | 5 | 6
  7 | 8 | 9
  */
class manage{
public:
	//对象管理
	int num;
	object * fp;
	//玩家位置
	int grid;
	float x;
	float y;//逻辑坐标
	



};

class object{
public:
	manage * parent;//通过指针获得上级信息
	int grid;//大网格下的位置
	float physic_x;//物理坐标
	float physic_y;//物理坐标
	float x;//逻辑坐标
	float y;//逻辑坐标
	float speed;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	


public:
	void appear();//随机出现
	void amaze();//吓人的动画？
};


