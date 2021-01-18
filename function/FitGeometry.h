//
// Created by lidan on 16/1/2021.
//

#ifndef BEZIERANDBSPLINE_FITGEOMETRY_H
#define BEZIERANDBSPLINE_FITGEOMETRY_H

#include <vector>
#include <Eigen/Dense>

#include "bspline.h"
using namespace Eigen ;

class InterpolationCurve{
private:
    std::vector<Vector3d> _points ;
    std::vector<Vector3d> _controlPoints ;
    Matrix<double,Dynamic,Dynamic> N;
    Bspline* bspline ;
public:
    InterpolationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points) ;
    std::vector<Vector3d>& getPoints(int Precisions) ;
};



#endif //BEZIERANDBSPLINE_FITGEOMETRY_H
