# -*- coding: utf-8 -*-

from numpy import *

#单层决策树生成

def loadSimpleData():
    datMat=matrix([[1.,2.1],
                [2.,1.1],
                [1.2,1.],
                [1. , 1.],
                [2.,1.]])
    classLabels=[1.0,1.0,-1.0,-1.0,1.0]
    return datMat,classLabels

# 行向量表示输入向量
# 列数 表示 组数
def stumpClassify(dataMatrix,dimen,threshVal,threshIneq):
    retArray=ones((shape(dataMatrix)[0],1))#生成一个对应结果列向量[ans]T
    if threshIneq=='lt':
        retArray[dataMatrix[:,dimen]<=threshVal]=-1.0
    else:
        retArray[dataMatrix[:,dimen]>threshVal]=-1.0
    return retArray

def buildStump(dataArr,classLabels,D):
    dataMatrix=mat(dataArr);labelMat=mat(classLabels).T
    m,n=shape(dataMatrix)
    numSteps=10.0;bestStump={};bestClasEst=mat(zeros((m,1)))
    minError=inf
    for i in range(n): # 遍历输入向量的每一维度，并且查找最大值、最小值
        rangeMin=dataMatrix[:,i].min(); rangeMax=dataMatrix[:,i].max()
        stepSize=(rangeMax-rangeMin)/numSteps
        for j in range(-1,int(numSteps)+1): # 在最大值和最小值之间分值
            for inequal in ['lt','gt']: # 分格
                threshVal=(rangeMin+float(j)*stepSize)
                predictedVals=\
                    stumpClassify(dataMatrix,i,threshVal,inequal)
                errArr=mat(ones((m,1)))
                errArr[predictedVals==labelMat]=0 # 预测正确的置零 错误置1
                weightedError=D.T*errArr  # 计算误差
                #print("split:dim %d, thresh %.2f, thresh ineqal: \
                #  %s ,the weighted error is %.3f " % \
                 # (i,threshVal,inequal,weightedError))
                if weightedError < minError :
                    minError=weightedError
                    bestClasEst=predictedVals.copy()
                    bestStump['dim']=i # 保存维数
                    bestStump['thresh']=threshVal # 保存分割线
                    bestStump['ineq']=inequal # 保存 方向（大于、小于）
    return bestStump,minError,bestClasEst

def adaBoostTrainDS(dataArr,classLabels,numIt=40):
    weakClassArr=[] # 分类器队列
    m=shape(dataArr)[0] # m 维数
    D=mat(ones((m,1))/m) # 分布律 均匀分布
    aggClassEst=mat(zeros((m,1))) # H(x)
    for i in range(numIt):
        bestStump,error,classEst=buildStump(dataArr,classLabels,D)
        print("D:",D.T)
        alpha=float(0.5*log((1.0-error)/max(error,1e-16))) # 修正alpha
        bestStump['alpha']=alpha # 分类器前的系数
        weakClassArr.append(bestStump) # 加入分类器
        print("classEst: ",classEst.T)
        expon=multiply(-1*alpha*mat(classLabels).T,classEst) #classEst : 分类器的预测结果
        D=multiply(D,exp(expon))
        D=D/D.sum() # 归一化分布函数
        aggClassEst+=alpha*classEst # Hn(x)=Hn-1(x)+alpha*ht(x)
        print("aggClassEst: ",aggClassEst.T)
        # 计算误差率
        aggErrors=multiply(sign(aggClassEst)!=mat(classLabels).T,ones((m,1))) # 每一项误差 * 均匀分布; 原输入误差
        errorRate=aggErrors.sum()/m
        print("total error: ",errorRate,"\n")
        if errorRate == 0.0 :break
    return weakClassArr
