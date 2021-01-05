//
// Created by lidan on 24/10/2020.
//

#ifndef BEZIERANDBSPLINE_MYMATH_H
#define BEZIERANDBSPLINE_MYMATH_H

#include <cmath>
#include <Eigen/Dense>
using namespace Eigen ;
using namespace std;
// 返回一个阶乘项
static inline int factorial(int n){
    int r = 1;
    for(int i = n; i>0; i--){
        r *= i;
    }
    return r;
}

// 返回一个伯恩斯坦基
static inline float bernstein(int i, int n, float t){
    float r = (float) factorial(n) / (float) (factorial(i) * factorial(n - i));
    r *= pow(t,i);
    r *= pow(1-t,n-i);
    return r;
}

static inline Vector3d* deCalsteljau(Vector3d* points,int degree,float t)
{
    float* pointsQ = new float[(degree+1)*3] ;
    int Qwidth = 3 ;
    for (int j = 0; j < degree ; ++j)
    {
        pointsQ[j*Qwidth+0] = points[j][0] ;
        pointsQ[j*Qwidth+1] = points[j][1] ;
        pointsQ[j*Qwidth+2] = points[j][2] ;
    }

    for(int m = 1; m< degree;m++)
    {
        for(int n = 0 ;n < degree-m;n++)
        {
            pointsQ[n*Qwidth+0] = pointsQ[n*Qwidth+0]*(1-t) + t* pointsQ[(n+1)*Qwidth+0] ;
            pointsQ[n*Qwidth+1] = pointsQ[n*Qwidth+1]*(1-t) + t* pointsQ[(n+1)*Qwidth+1] ;
            pointsQ[n*Qwidth+2] = pointsQ[n*Qwidth+2]*(1-t) + t* pointsQ[(n+1)*Qwidth+2] ;
        }
    }

    Vector3d* result =new Vector3d ;
    *result<<pointsQ[0],pointsQ[1],pointsQ[2] ;


    delete pointsQ ;
    return result ;
}

#endif //BEZIERANDBSPLINE_MYMATH_H
