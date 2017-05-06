#include <stdio.h>
#include "highgui.h"
#include <math.h>

void Inf(IplImage * img,float mean[],float st_deviation[] ){//返回各通道平均值和标准差
	int x=img->width;
	int y=img->height;
	int step=img->widthStep;

	int i,j;

	double block;//x*y
	double r_total,g_total,b_total;
	double r_sq,g_sq,b_sq;

	unsigned char *fp;

	block=x*y;
	if (step/x==3)//RGB 三通道
	{
		r_total=r_sq=0;
		g_total=g_sq=0;
		b_total=b_sq=0;
		for (i=0;i<y;i++)//刷行
			for (j=0;j<x;j++)//刷列
			{
				fp=(unsigned char *)(img->imageData+i*step+j*3);
				r_total+=*fp;r_sq+=(*fp)*(*fp);
				fp++;
				g_total+=*fp;g_sq+=(*fp)*(*fp);
				fp++;
				b_total+=*fp;b_sq+=(*fp)*(*fp);
			}
		mean[0]=r_total/block;
		mean[1]=g_total/block;
		mean[2]=b_total/block;
		st_deviation[0]=sqrt(r_sq/block-mean[0]*mean[0]);
		st_deviation[1]=sqrt(g_sq/block-mean[1]*mean[1]);
		st_deviation[2]=sqrt(b_sq/block-mean[2]*mean[2]);
	}
	else //单通道
	{
		r_total=r_sq=0;
		for (i=0;i<y;i++)//刷行
			for (j=0;j<x;j++)//刷列
			{
				fp=(unsigned char *)(img->imageData+i*step+j);
				r_total+=*fp;r_sq+=(*fp)*(*fp);
			}
		mean[0]=r_total/block;
		st_deviation[0]=sqrt(r_sq/block-mean[0]*mean[0]);
	}
}

void DB(IplImage * img,float contrast=1.0,float brightness=1.0){//对比度 明度调节
	int x=img->width;
	int y=img->height;
	int step=img->widthStep;
	
	int i,j;

	double block;//x*y
	double r_total,g_total,b_total;
	double r_average,g_average,b_average;
	double rup_average,gup_average,bup_average;
	unsigned char * fp;
	int temp;

	block=x*y;
	if (step/x==3)//RGB 三通道
	{
		r_total=0;
		g_total=0;
		b_total=0;
		for (i=0;i<y;i++)//刷行
			for (j=0;j<x;j++)//刷列
			{
				fp=(unsigned char *)(img->imageData+i*step+j*3);
				r_total+=*fp;
				fp++;
				g_total+=*fp;
				fp++;
				b_total+=*fp;
			}
		r_average=r_total/block;
		g_average=g_total/block;
		b_average=b_total/block;
		
		rup_average=r_average*brightness;
		gup_average=g_average*brightness;
		bup_average=b_average*brightness;

		for (i=0;i<y;i++)//刷行
			for(j=0;j<x;j++)//刷列
			{
				fp=(unsigned char *)(img->imageData+i*step+j*3);
				temp=(*fp-r_average)*contrast+rup_average,*fp=temp>255 ? 255: (temp<0 ? 0 : temp);
				fp++;
				temp=(*fp-b_average)*contrast+bup_average,*fp=temp>255 ? 255: (temp<0 ? 0 : temp);
				fp++;
				temp=(*fp-g_average)*contrast+gup_average,*fp=temp>255 ? 255: (temp<0 ? 0 : temp);
			}	
	}
	else//单通道
	{
		r_total=0;
		for (i=0;i<y;i++)
			for (j=0;j<x;j++){
				fp=(unsigned char *)(img->imageData+i*step+j);
				r_total+=*fp;
			}
		r_average=r_total/block;

		rup_average=r_average*brightness;

		for(i=0;i<y;i++)//刷行
			for(j=0;j<x;j++)//刷列
			{
				fp=(unsigned char *)(img->imageData+i*step+j);
				temp=(*fp-r_average)*contrast+rup_average,*fp=temp>255 ? 255: (temp<0 ? 0 : temp);
			}
		
	}
	return;
}

void Cal(IplImage * img,float * contrast,float * brightness,float mean,float st_deviation){//计算明度 饱和度调节值
	float r_g_b_mean[3];
	float r_g_b_d[3];

	r_g_b_mean[1]=-1;
	Inf(img,r_g_b_mean,r_g_b_d );//获得参数
	if (r_g_b_mean[1]<0){//单通道
		*brightness=mean/r_g_b_mean[0];
		*contrast=st_deviation/r_g_b_d[0];
	}
	else//三通道
	{
		double st_all;
		*brightness=mean*3/(r_g_b_mean[0]+r_g_b_mean[1]+r_g_b_mean[2]);
		st_all=pow(r_g_b_d[0],2)+pow(r_g_b_d[1],2)+pow(r_g_b_d[2],2);
		*contrast=st_deviation/sqrt(st_all/3);
	}
	return;
}