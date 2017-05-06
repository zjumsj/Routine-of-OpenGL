# -*- coding: utf-8 -*-
from math import *
import numpy as np
from PIL import Image

#参数设置 每个小单元
w=1 #矩形宽
l=1 #矩形高



def compare(A,B):
    a=A.sum()
    b=B.sum()
    if (a>b):
        return 1
    else:
        return 0

def cal(X,i,j):
    sum=0
    cen=X[i:i+w,j:j+l]
    sum+=128*compare(cen,X[i-w:i,j-l:j]) # 左上
    sum+=64*compare(cen,X[i:i+w,j-l:j])  # 上
    sum+=32*compare(cen,X[i+w:i+2*w,j-l:j]) # 右上
    #
    sum+=1*compare(cen,X[i-w:i,j:j+l]) # 左
    sum+=16*compare(cen,X[i+w:i+2*w,j:j+l]) # 右
    #
    sum+=2*compare(cen,X[i-w:i,j+l:j+2*l]) # 左下
    sum+=4*compare(cen,X[i:i+w,j+l:j+2*l]) # 下
    sum+=8*compare(cen,X[i+w:i+2*w,j+1:j+2*l]) # 右下
    ## Do not apply Rotation Invalience
    #return sum
    ## Apply Rotational Invalience
    maxn=sum
    for i in range(1,8): #Shifting
        p=((sum>>i)+(sum<<(8-i))) & 255
        if p>maxn:
            maxn=p
    #return floor(255*(maxn/255)**3)
    return maxn

def LAB(arr):
    width=arr.shape[0]
    height=arr.shape[1]
    xup=floor(width/w)
    yup=floor(height/l)
    z=np.zeros((xup,yup)) #Establish a new array
    # avoid edge
    for i in range(1,xup-1):
        for j in range(1,yup-1):
            z[i,j]=cal(arr,i*w,j*l)
    return z


def load(path):
    img=Image.open(path)
    #img.show()
    img=img.convert('L') # convert to gray
    arr=np.asarray(img,dtype='ubyte')
    return arr


if __name__=='__main__':
    arr=load('B2_3.jpg')
    arr2=LAB(arr)
    img=Image.fromarray(arr2)
    img.show()
