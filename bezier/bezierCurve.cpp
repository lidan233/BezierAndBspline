//
// Created by lidan on 24/10/2020.
//

#include "bezierCurve.h"
#include "MyMath.h"
#define ALGORITHM 0


Vector3d* bezierCurve::calCurve() {
    std::cout<< "New Line"<<std::endl ;
    for(int i = 0 ;i < numPoints;i++)
    {
        std::cout<<controlPoints[i][0]<<","<<controlPoints[i][1]<<","<<controlPoints[i][2]<<std::endl ;
    }

    curvePoints = new Vector3d[precision+1] ;
    if(ALGORITHM == 0)
    {
        for(int i = 0 ;i <= precision ;i++)
        {
            float u = float(i) /float(precision) ;
            curvePoints[i] = *(deCalsteljau(controlPoints, numPoints, u)) ;
        }
    }else{
        for(int i = 0; i <= precision; i++) {
            float u = float(i) / float(precision);
            Vector3d *point = new Vector3d;
            *point<< 0,0,0;

            for (int m = 0; m <= numPoints; m++) {
                float bm = bernstein(m, numPoints, u);

                (*point)[0] += bm * controlPoints[m][0];
                (*point)[1] += bm * controlPoints[m][1];
                (*point)[2] += bm * controlPoints[m][2];
            }
            curvePoints[i] = *point;
        }
    }
    return curvePoints ;
}
bool bezierCurve::setshowPoints(int showControlPoints) {
    return false ;
}

bezierCurve::bezierCurve( Vector3d *points,int numPoints):bezier() {
    this->controlPoints = points ;
    this->numPoints = numPoints ;
}

void bezierCurve::draw() {

}

bezierCurve::~bezierCurve() {

}

