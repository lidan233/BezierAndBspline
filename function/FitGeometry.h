//
// Created by lidan on 16/1/2021.
//

#ifndef BEZIERANDBSPLINE_FITGEOMETRY_H
#define BEZIERANDBSPLINE_FITGEOMETRY_H

#include <vector>
#include <Eigen/Dense>

#include "bspline.h"
#include "bplineSurface.h"


using namespace Eigen ;

class InterpolationCurve{
private:
    std::vector<Vector3d> _points ;
    std::vector<Vector3d> _controlPoints ;
    Matrix<double,Dynamic,Dynamic> N;
    Bspline* bspline ;
public:
    InterpolationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points) ;
    std::vector<Vector3d>& getPoints(int Precisions, bool isCox) ;
};

class ApproximationCurve{
private:
    std::vector<Vector3d> _points ;
    std::vector<Vector3d> _controlPoints ;
    Matrix<double, Dynamic,Dynamic> N ;
    Bspline* bspline ;

public:
    ApproximationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points) ;
    std::vector<Vector3d>& getPoints(int Precisions, bool isCox) ;
};

class InterpolationSurface{
private:
    std::vector<std::vector<Vector3d> > _points ;
    std::vector<std::vector<Vector3d> > _controlPoints ;

    int degreeall = 0 ;

public:
    InterpolationSurface(bool isCox, int degreeall, std::vector<std::vector<float>>& knots, std::vector<std::vector<Vector3d>>& points,int PrecisionX,int PrecisionY) ;
//    std::vector<Vector3d> getTriangles(bool isCox) ;
//    std::vector<Vector3d> getLines(bool isCox) ;
    std::vector<Vector3d > getPoints() {
        std::vector<Vector3d> rr ;
        for(int i = 0 ; i < _controlPoints.size(); i++)
        {
            for(int j = 0 ; j < _controlPoints[i].size();j++)
            {
                rr.push_back(_controlPoints[i][j]) ;
            }
        }
        return rr ;
    }

};


#endif //BEZIERANDBSPLINE_FITGEOMETRY_H
