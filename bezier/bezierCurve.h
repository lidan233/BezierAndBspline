//
// Created by lidan on 24/10/2020.
//

#ifndef BEZIERANDBSPLINE_BEZIERCURVE_H
#define BEZIERANDBSPLINE_BEZIERCURVE_H

#include "bezier.h"
#include <Eigen/Dense>
#include <iostream>
using namespace Eigen ;


class bezierCurve: public bezier{
private:
    int numPoints ;
    Vector3d *controlPoints ;
    Vector3d *curvePoints ;


public:
    bezierCurve(Vector3d* controlPoints,int numPoints ) ;
    bool setshowPoints(int  showControlPoints ) ;
    virtual void draw();
    Vector3d* calCurve(bool isdeCalsteljau) ;
    ~bezierCurve();
};
#endif //BEZIERANDBSPLINE_BEZIERCURVE_H
