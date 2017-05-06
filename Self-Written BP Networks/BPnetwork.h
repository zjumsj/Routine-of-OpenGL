#include <math.h>
#include <float.h>
#include <windows.h>  
#include <stdlib.h>
#include <stdio.h>

class BPlayer;

double defaultF(double _X);
double defaultdF(double _X,double _Y);




class BPnetwork{

friend class BPlayer;//声明友元类

private:
//MAIN
int _depth;
bool zero;
bool trained;
BPlayer ** _fp;//指针数组
double (* F)(double _X);//定义函数指针
double (* dF)(double _X,double _Y);//定义函数指针

//Tag
int _Tmax;//最大训练次数
double _err;//目标误差

//TEMP
int _width;
int _height;//实际输入高度
double * _input;
int _output;


public://成员函数

  int Depth(void);//返还深度
//新建一个BP
  BPnetwork(int depth=2,int * num_fp=NULL); //构造
  ~BPnetwork();//定义一个析构函数
  void BPchoosefunction(double (* bpf)(double _X),double (* bpdf)(double _X,double _Y));//选择函数
  bool BPpushlayer(int num);//新建最后一层
  bool BPpoplayer(int num);//删除最后一层
  bool BPdelete(int index);//删除神经元
  bool BPinsert(int index);//插入神经元
  int  BPTrain(double * input,int width,int height,double * rate=NULL);//按照矩阵形式输入，最后一行是输出
  bool BPBroadCast(double * input,int width);
  void SetBPConfig(int T,double err);

private:
	double CalculateError(int clamp);

  

};

class BPlayer{
	friend class BPnetwork;
private:
	int _num;int _former;
	int _index;//在BPnetwork的制定层数
	double * _w , * _nw;
	double * _theta;
	double * _s;//S
	double * _ans;//F(S)
	double * _diedai;//迭代空间
	bool zero;


    BPnetwork * _f;

private://保护 不能单独使用
	BPlayer(int index,int num,BPnetwork * f);
	void Establish(int former);
	void FeedForward(int clamp);
	void BackPropagate(int clamp,double lr);
	void Refresh(void);
	~BPlayer();


};

