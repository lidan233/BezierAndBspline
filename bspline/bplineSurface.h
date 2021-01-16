//
// Created by lidan on 29/10/2020.
//

#ifndef BEZIERANDBSPLINE_BPLINESURFACE_H
#define BEZIERANDBSPLINE_BPLINESURFACE_H


#include <Eigen/Dense>
#include <math.h>
#include <list>
#include <vector>
#include <bspline.h>
using namespace std ;
using namespace Eigen ;


class bSplineSurface {
private:
    Vector3d** _controlPoints ;
    int xsize ;
    int ysize ;
    Vector3d** _points = nullptr ;
    int degree ;

    int PrecisionSize_X = 0 ;
    int PrecisionSize_Y = 0 ;

    std::vector<Vector3d> calculate_Y_curve_By_deboor(int i) ;
    std::vector<Vector3d> calculate_Y_curve_By_Cox_deboor(int i) ;

public:
    bSplineSurface(Vector3d** controlPoints,const unsigned int xsize , const unsigned int ysize ,int degree) ;
    void getCubicYPoint(float ty,int xorder,int yorder, Vector3d& res) ;
    void getCubicPoint(float tx,float ty,int xorder,int yorder,Vector3d& res) ;
    void setPrecision(int precisionX, int precisionY) ;
    void getCubicPoints() ;


    Vector3d** calSurface_By_deboor() ;
    Vector3d** calSurface_By_Cox_deboor() ;

    std::vector<Vector3d> getTriangles() ;
    std::vector<Vector3d> getLines() ;
};


#endif //BEZIERANDBSPLINE_BPLINESURFACE_H
