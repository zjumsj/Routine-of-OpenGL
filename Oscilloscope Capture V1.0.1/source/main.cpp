#include "stdio.h"
#include "highgui.h"
#include "cv.h"
#include "math.h"

#define CV_GET_SEQ_ELEMENT(a,b,c)  *((a *)cvGetSeqElem(b,c))

#define WIDTH  920
#define HEIGHT 690

int N;


void Judge(CvSeq * fp,IplImage * img_information,CvPoint  st[]);
void DB(IplImage * img,float contrast,float brightness);
void Inf(IplImage * img,float mean[],float st_deviation[] );
void Cal(IplImage * img,float * contrast,float * brightness,float mean,float st_deviation);

int main(void){

	unsigned char Err=0;

	CvMat * Cha=cvCreateMat(3,3,CV_32FC1);
	CvMat * Cha2=cvCreateMat(2,3,CV_32FC1);
	CvSize cvsz;
	float fx,fy;//裁剪系数
	int index;double MAX;
	CvSeq * contours, * contour,* maxcontour;
	CvPoint st[4];
	CvPoint2D32f  QuS[4],QuT[4];//Source Target
	CvMemStorage* storage=cvCreateMemStorage(0);
	char PATH[10];
	char PATHH[10];
	PATH[1]='.',PATHH[1]='P',PATHH[2]='.';
	PATH[2]=PATHH[3]='j';
	PATH[3]=PATHH[4]='p';
	PATH[4]=PATHH[5]='g';
	PATH[5]=PATHH[6]=0;

	//初始化裁剪系数
	fx=0.012;
	fy=0.02;

	//欢迎界面
	printf("欢迎使用示波器图形抓图软件\n");
	printf("提示：请在文件夹里安置文件命名如下:\n");
	printf("1.jpg   2.jpg   3.jpg  ...");
	printf("总共输入1-N?:  ");
	scanf("%d",&N);

	int ii;

	for ( ii=1;ii<N+1;ii++){
	PATH[0]=ii+'0';
	IplImage* img_pre=cvLoadImage(PATH);//载入图片
	
	if (img_pre==NULL){
		printf("Picture %d Is Not Found!\n",ii);
		continue;
	}

	IplImage * img=cvCreateImage(cvSize(960,720),IPL_DEPTH_8U,3);//获得处理前图像
	
	
	//预处理
	QuS[0].x=0;QuS[0].y=0;
	QuS[1].x=img_pre->width;QuS[1].y=0;
	QuS[2].x=0;QuS[2].y=img_pre->height;

	QuT[0].x=0;QuT[0].y=0;
	QuT[1].x=960;QuT[1].y=0;
	QuT[2].x=0;QuT[2].y=720;

	cvGetAffineTransform(QuS,QuT,Cha2);
	cvWarpAffine(img_pre,img,Cha2);	

	//回收img_pre
	cvReleaseImage(& img_pre);
	//主处理
	cvsz=cvGetSize(img);//获取尺寸

	IplImage * img2=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//创建灰度图像
	//Debug

	IplImage * img3=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//创建二值图像
	IplImage * img_out=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//创建定位图像
	IplImage * img_ans=cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3);//创建最终输出
	IplImage * img_ans2=cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3);//创建最终输出
	//初始化最终输出图片

	img_ans->origin=img->origin;
	cvZero(img_ans);
	//
	cvCvtColor(img, img2, CV_BGR2GRAY);//转换灰度图像
	
	//
	cvZero(img_out);
	
	if (Err){//错误时尝试
		Err=2;
		cvErode(img2,img2,NULL,1);
	}
	
	
	cvAdaptiveThreshold(img2,img3,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,7);//转化
	//回收img2
	cvReleaseImage(&img2);

	//
	cvClearMemStorage(storage);//清内存
	cvFindContours(img3,storage,&contours);//查找轮廓并且保存在contours里
		
	

	index=0;
	MAX=0;
	maxcontour=NULL;
	for (contour=contours;contour!=NULL;contour=contour->h_next,index++) // Error
    {	  
		double Area=cvContourArea(contour);//获得轮廓面积
		if (Area>MAX){//保存面积最大的指针
			maxcontour=contour;
			MAX=Area;
		}
	}		
	
	//Self_Adapting
	if (MAX<60000){//自动检测出错
		if (Err==2)//出错后自动跳过
		{
			cvReleaseImage(&img3);
			cvReleaseImage(&img_out);
			cvReleaseImage(&img_ans);
			cvReleaseImage(&img_ans2);
			printf("Picture %d Failed!\n",ii);
			Err=0;
			continue;
		}
		else{
			Err=1;
			cvReleaseImage(&img3);
			cvReleaseImage(&img_out);
			cvReleaseImage(&img_ans);
			cvReleaseImage(&img_ans2);
			printf("Picture %d Retry!\n",ii);
			ii--;
			continue;
		}		
	}
	else
		if (Err==2)//成功解决BUG
			Err=0;
	//End Self_Adapting

	Judge(maxcontour,img_out,st);//自动查找边缘点函数

	//开始产生特征矩阵,并且进行透视变换
	QuS[0].x=st[0].x;QuS[0].y=st[0].y;
	QuS[1].x=st[1].x;QuS[1].y=st[1].y;
	QuS[2].x=st[2].x;QuS[2].y=st[2].y;
	QuS[3].x=st[3].x;QuS[3].y=st[3].y;
	QuT[0].x=QuT[0].y=0;
	QuT[1].x=WIDTH;QuT[1].y=0;
	QuT[2].x=0;QuT[2].y=HEIGHT;
	QuT[3].x=WIDTH,QuT[3].y=HEIGHT;

	cvGetPerspectiveTransform(
			QuS,
			QuT,
			Cha
		);
	cvWarpPerspective(img,img_ans,Cha);
	//开始进行仿射变换
	QuS[0].x=fx*WIDTH;QuS[0].y=fy*HEIGHT;
	QuS[1].x=(1-fx)*WIDTH;QuS[1].y=fy*HEIGHT;
	QuS[2].x=(1-fx)*WIDTH;QuS[2].y=(1-fy)*HEIGHT;
		
	QuT[0].x=0;QuT[0].y=0;
	QuT[1].x=WIDTH;QuT[1].y=0;
	QuT[2].x=WIDTH;QuT[2].y=HEIGHT;
	
	cvGetAffineTransform(QuS,QuT,Cha2);

	cvWarpAffine(img_ans,img_ans2,Cha2);
	cvXorS(img_ans2, cvScalarAll(255), img_ans2);
	
	//INSERT 2
	float contrast;
	float brightness;
	Cal(img_ans2,&contrast,&brightness,220,75);
	DB(img_ans2,contrast,brightness);


	//Save The Picture
	printf("Picture %d Succeed!\n",ii);
	PATHH[0]='0'+ii;
	cvSaveImage(PATHH,img_ans2);
	//Release
	cvReleaseImage(&img3);
	cvReleaseImage(&img_out);
	cvReleaseImage(&img_ans);
	cvReleaseImage(&img_ans2);
	}
	
	cvReleaseMemStorage(&storage);
	cvReleaseMat(&Cha);
	printf("Finish   Press Any Key To Exit\n");
	getchar();
	getchar();
	return 0;

}

void Judge(CvSeq * fp,IplImage * img_information,CvPoint  st[]){
	int x=img_information->width;
	int y=img_information->height;
	int M=fp->total;
	int i;
	int factor=-0.7;//参数影响因子
	CvPoint ic;
	double MI[4]={1e9,1e9,1e9,1e9};
	double r2;

	for (i=0;i<M;i++){
		ic=CV_GET_SEQ_ELEMENT(CvPoint,fp,i);//获得CvPoint类型
		if(ic.x<x/2 && ic.y<y/2)//第一象
		{
			r2=fabs(ic.x)+fabs(ic.y)+factor*sqrt(ic.x*ic.x+ic.y*ic.y);
			if(r2<MI[0]){
				st[0]=ic;
				MI[0]=r2;
			}
		}
		else if(ic.y<y/2)//第二象
		{
			r2=fabs(ic.x-x)+fabs(ic.y)+factor*sqrt(pow(x-ic.x,2)+pow(ic.y,2));
			if (r2<MI[1]){
				st[1]=ic;
				MI[1]=r2;			
			}		
		}
		else if (ic.x<x/2)//第三象
		{
			r2=fabs(ic.x)+fabs(ic.y-y)+factor*sqrt(pow(ic.x,2)+pow(y-ic.y,2));
			if (r2<MI[2]){
				st[2]=ic;
				MI[2]=r2;			
			}
		}
		else//第四象
		{
			r2=fabs(ic.x-x)+fabs(ic.y-y)+factor*sqrt(pow(x-ic.x,2)+pow(y-ic.y,2));
			if (r2<MI[3]){
				st[3]=ic;
				MI[3]=r2;
			}
		}
	}
}
