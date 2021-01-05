//
// Created by lidan on 29/10/2020.
//

#ifndef BEZIERANDBSPLINE_BPLINESURFACE_H
#define BEZIERANDBSPLINE_BPLINESURFACE_H


#include <Eigen/Dense>
#include <math.h>
#include <list>
#include <vector>

using namespace std ;
using namespace Eigen ;


class bSplineSurface {
private:
    Vector2d** _controlPoints ;
    int xsize ;
    int ysize ;
    Vector2d** _points ;
    int degree ;

public:
    bSplineSurface(Vector2d** controlPoints,const unsigned int xsize , const unsigned int ysize ,int degree) ;
    void getYPoint(float ty,int xorder,int yorder, Vector2d& res) ;
    void getPoint(float tx,float ty,int xorder,int yorder,Vector2d& res) ;
    void getPoints(int precisionx,int precisiony ) ;
    void getPointsByRPoint(int num) ;
    void getPointsByRBasis(int num) ;
};


#endif //BEZIERANDBSPLINE_BPLINESURFACE_H
