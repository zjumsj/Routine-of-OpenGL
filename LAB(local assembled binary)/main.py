# -*- coding: utf-8 -*-
from numpy import *
import AdaBoost as ab
import LAB as lab


if __name__=='__main__':

    mylist=[];mytrain=[]
    # 载入网球
    for index in range(5):
        arr=lab.load('B0_%d.jpg' % index)
        arr2=lab.LAB(arr).flatten() # 转化后展平
        mylist.append(arr2.tolist())
        mytrain.append(1.)
    # 载入苹果
    for index in range(5):
        arr =lab.load('B2_%d.jpg' % index)
        arr2=lab.LAB(arr).flatten() # 转化后展平
        mylist.append(arr2.tolist())
        mytrain.append(-1.)
    #
    dataArr=matrix(mylist)
    classLabels=matrix(mytrain)
    D=mat(ones((10,1))/10)
    classifierArray=ab.adaBoostTrainDS(dataArr,classLabels,10)
    for x in classifierArray:
        print('alpha:',x['alpha'],
          'dim:',x['dim'],
          'thresh:',x['thresh'],
          'ineq:',x['ineq']
          )




