#include "stdafx.h"
#include "BPnetwork.h"


double defaultF(double _X)//default Sigmoid Function
{
    return 1/(1+exp(-_X));

}

double defaultdF(double _X,double _Y){
	return (1-_Y)*_Y;

}

int BPnetwork::Depth(void){return _depth;}

void BPnetwork::SetBPConfig(int T,double err){//设置运行参数
	_Tmax=T;
	_err=err;
}
BPnetwork::BPnetwork(int depth,int *num_fp){//新建
	
	if (depth<1){
		zero=true;
		_depth=0;
		_fp=NULL;
	}
	else
	{
		int i;
		_depth=depth;//Initialize
		_fp=new BPlayer * [depth];
//
		if (num_fp==NULL){
			for (i=0;i<depth;i++){
				_fp[i]=new BPlayer(i,20,this);//默认层数
			}
		}
		else{
			for (i=0;i<depth;i++){
				_fp[i]=new BPlayer(i,num_fp[i],this);		
			}	
		}
		zero=false;
	}
	//Initialize
	F=defaultF;
	dF=defaultdF;
	_Tmax=100;
	_err=0.1;
	trained=false;
	return;
}

BPnetwork::~BPnetwork(){//析构函数
	if (zero==false){
		int i;
		for (i=0;i<this->_depth;i++)
			delete _fp[i];
		delete [] _fp;
	}
}

void BPnetwork::BPchoosefunction(double (* bpf)(double _X),double (* bpdf)(double _X,double _Y)){//设置S函数
	F=bpf;
	dF=bpdf;

	return;
}
bool BPnetwork::BPdelete(int index){


//Unwritten

	return true;
}
bool BPnetwork::BPinsert(int index){

//Unwritten

	return true;
}
bool BPnetwork::BPpoplayer(int num){
	int i;
	if (_depth==1){
		_depth=0;
		delete _fp[0];
		delete [] _fp;
		_fp=NULL;
		zero=true;
		return true;
	}
	if (zero==false){
		BPlayer ** temp;
		_depth--;
		temp=new BPlayer * [_depth];
		delete _fp[_depth];
		for (i=0;i<_depth;i++)
			temp[i]=_fp[i];
		delete [] _fp;
		_fp=temp;
		return true;
	}

	trained=false;//结构改变
	return false;
}
bool BPnetwork::BPpushlayer(int num){
	int i;
	BPlayer ** temp;
	temp=new BPlayer * [_depth+1];
	for(i=0;i<_depth;i++)
		temp[i]=_fp[i];
	delete [] _fp;
	_fp=temp;
	temp[_depth]=new BPlayer(_depth,num,this);
	_depth++;

	trained=false;//结构改变
	return true;
}
int BPnetwork::BPTrain(double * input,int width,int height,double * rate){//返还训练次数
	
	_output=_fp[_depth-1]->_num;//输出个数
	_height=height-_output;//实际输入

	if (zero==true)//无神经元网络建立
		return -1;
	if (width<1 || _height<1 || _output<1)//出错
		return -1;

	//Copy
	_input=input;
	_width=width;
	
	double * repl=new double[_depth];
	int i;
	int j;
	int k;
	double TotalError;

	if (rate==NULL){
		rate=repl;
		for (i=0;i<_depth;i++){//设置一个默认学习速率
			rate[i]=0.001;//		
		}
	}

	_fp[0]->Establish(_height);
	for (i=1;i<_depth;i++)
		_fp[i]->Establish(_fp[i-1]->_num);

		
	TotalError=DBL_MAX;
	for(i=0;i<_Tmax;i++){//学习过程
		if (TotalError<_err)
			break;

		TotalError=0;
		for (k=0;k<width;k++){//clamp each input

			for(j=0;j<_depth;j++){
				_fp[j]->FeedForward(k);//前向传递
			}
			TotalError+=CalculateError(k);//计算误差

			for(j=_depth-1;j>-1;j--){//向后传递
				_fp[j]->BackPropagate(k,rate[j]);//调整权值			
			}
			for (j=0;j<_depth;j++){
				_fp[j]->Refresh();//更新			
			}
		}
	}
	delete [] repl;
	trained=true;
	return i;
}
bool BPnetwork::BPBroadCast(double * input,int width){

	if (trained==false || zero==true)
		return false;
	
	int i;int j;
	_input=input;
	_width=width;

	BPlayer * last=_fp[_depth-1];
	for (i=0;i<width;i++){//i is clamped
		for (j=0;j<_depth;j++){//j 神经元层数
			_fp[j]->FeedForward(i);	
		}
		//取出数据
		for (j=0;j<_output;j++){// 最后一层神经元逐个取值
			input[(_height+j)*width+i]=last->_ans[j];		
		}
	}
	return true;
}
double BPnetwork::CalculateError(int clamp){//1/2差的平方 能量函数
	double ANS=0;
	int i;
	for (i=0;i<_output;i++){
		ANS+=0.5*pow(_fp[_depth-1]->_ans[i]-_input[(i+_height)*_width+clamp],2);	
	}
	return ANS;
}
//---------Here are codes for BPlayer---------------

BPlayer::BPlayer(int index,int num,BPnetwork * f){//Initialize
	_num=num;
	_index=index;
	_f=f;
	zero=true;
}
void BPlayer::Establish(int former){//新建
	int i;
	_former=former;
	_w=new double[former*_num];//建立新的权空间
	_nw=new double[former * _num];//备用
	_theta=new double[_num];//新的

	//随机数生成
	LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);//初始化一个以微秒为单位的时间种子
    //

	for (i=0;i<former*_num;i++)
		_w[i]=0.001*(rand()%1000)-0.5;

	for (i=0;i<_num;i++)
		_theta[i]=0.001*(rand()%1000)-0.5;
	

	_s=new double[_num];//S
	_ans=new double[_num];//结果的存储空间
	_diedai=new double [_num];//迭代空间
	zero=false;

}
BPlayer::~BPlayer(){//回收空间
	if (!zero){
		delete [] _w; delete [] _nw;
		delete [] _theta;
		delete [] _ans;
		delete [] _s;
		delete [] _diedai;
	}
}
void BPlayer::BackPropagate(int clamp,double lr){//学习速率 逐级迭代
	
	double * former;
	int i;
	int j;

	former=new double[_former];
	
	//取出上级数据
	if (_index==0){
		int width=_f->_width;

		for (i=0;i<_former;i++){
			former[i]=_f->_input[clamp+i*width];		
		}
	}
	else{
		
		BPlayer * temp;
		temp=_f->_fp[_index-1];
		
		for(i=0;i<_former;i++){
			former[i]=temp->_ans[i];
		}  
	}
	//
	//正式开始迭代


	if (_index==_f->_depth-1){//零级迭代
		
		int height=_f->_height;
		int width=_f->_width;

		for (i=0;i<_num;i++){
			_diedai[i]=(-_ans[i]+_f->_input[(height+i)*width+clamp])*_f->dF(_s[i],_ans[i]);
			//(Oi-Di)*dDi/dS

			//学习
			for (j=0;j<_former;j++)//wij
			{
				_nw[i*_former+j]=_w[i*_former+j]+lr*_diedai[i]*former[j];
			}
			_theta[i]+=lr*_diedai[i];
		}	
	}
	else{//深层迭代
		BPlayer * bpl=_f->_fp[_index+1];
		int latter=bpl->_num;//取前一层元素数
	

		//求和 前一层迭代 dSn+1/dY dY/dSn 
		for (i=0;i<_num;i++){
			_diedai[i]=0;
			for (j=0;j<latter;j++){
				_diedai[i]+=bpl->_diedai[j] * bpl->_w[j*_num+i] * _f->dF(_s[i],_ans[i]);
			}
			
			//学习
			for (j=0;j<_former;j++)
			{
				_nw[i*_former+j]=_w[i*_former+j]+lr*_diedai[i]*former[j];
			}
			_theta[i]+=lr*_diedai[i];			
		}	
	}

	delete [] former;//回收内存
	
	return;
}
void BPlayer::FeedForward(int clamp){
	double * former;
	int i;
	int j;

	former=new double[_former];
	
	//取出上级数据
	if (_index==0){
		int width=_f->_width;

		for (i=0;i<_former;i++){
			former[i]=_f->_input[clamp+i*width];		
		}
	}
	else{
		
		BPlayer * temp;
		temp=_f->_fp[_index-1];
		
		for(i=0;i<_former;i++){
			former[i]=temp->_ans[i];
		}  
	}

	double SUM;
	//正式开始
	for (i=0;i<_num;i++)//当前层的每个神经元
	{
		SUM=0;
		for (j=0;j<_former;j++)//先前层的每个神经元
			SUM+=former[j]*_w[i*_former+j];
		SUM+=_theta[i];
		_s[i]=SUM;
		_ans[i]=_f->F(SUM);
	}

	delete [] former;//回收

}
void BPlayer::Refresh(void){//更新
	int i;
	for (i=0;i<_former*_num;i++){
		_w[i]=_nw[i];	
	}
}


