//
// Created by lidan on 24/10/2020.
//

#include "bezierSurface.h"
#include "MyMath.h"
#define ALGORITHM 0

void bezierSurface::calSurface() {
    curvePoints = new Vector3d*[precision+1] ;
    for(int i = 0 ;i < precision;i++)
    {
        curvePoints[i] = new Vector3d[precision+1] ;
    }
    if(ALGORITHM == 0)
    {
        for(int i = 0 ;i <=precision ;i++)
        {
            float u = float(i)/float(precision);
            for(int j = 0 ; j<= precision; j++)
            {
                float v = float(j)/float(precision) ;
                Vector3d* qpoints = new  Vector3d[precision+1] ;
                for( int k =0 ; k<= uPoints ;k++)
                {
                    qpoints[k] = *(deCalsteljau(controlPoints[k],vPoints,v)) ;
                }
                curvePoints[i][j] = *(deCalsteljau(qpoints,uPoints,u)) ;
            }
        }
    }else{
        for(int i = 0 ;i <= precision ;i++)
        {
            float u = float(i) / float(precision) ;
            for(int j = 0 ; j<= precision ;j++)
            {
                float v = float(j)/float(precision) ;
                float** qpoints = new float*[precision+1] ;


                Vector3d res ;
                for(int k = 0 ;k<= uPoints;k++)
                {
                    for(int m = 0 ; m<= vPoints;m++)
                    {
                        float bk = bernstein(k, uPoints, u);
                        float bm = bernstein(m, vPoints, v);
                        float b = bk * bm;
                        res[0] += b*controlPoints[k][m][0] ;
                        res[1] += b*controlPoints[k][m][1] ;
                        res[2] += b*controlPoints[k][m][2] ;
                    }
                }
                curvePoints[i][j] = res;
            }
        }
    }
}

void bezierSurface::draw() {

}

void bezierSurface::setShowPoints(bool showControlPoints) {

}

bezierSurface::bezierSurface(Vector3d** points ,int uPoints, int vPoints):bezier() {
    this->uPoints = uPoints ;
    this->vPoints = vPoints ;
    controlPoints = points ;
}

bezierSurface::~bezierSurface() {

}

