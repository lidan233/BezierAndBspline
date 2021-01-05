//
// Created by lidan on 24/10/2020.
//

#ifndef BEZIERANDBSPLINE_BEZIERSURFACE_H
#define BEZIERANDBSPLINE_BEZIERSURFACE_H
#include "bezier.h"
#include "Eigen/Dense"

using namespace Eigen ;

class bezierSurface: public bezier {
private:
    int uPoints;
    int vPoints;
    Vector3d **controlPoints;
    Vector3d **curvePoints;

public:
    bezierSurface(Vector3d** points ,int uPoints, int vPoints);
    void setShowPoints(bool showControlPoints);
    virtual void draw();
    void calSurface();
    ~bezierSurface();
};


#endif //BEZIERANDBSPLINE_BEZIERSURFACE_H
