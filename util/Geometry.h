//
// Created by lidan on 16/1/2021.
//

#ifndef BEZIERANDBSPLINE_GEOMETRY_H
#define BEZIERANDBSPLINE_GEOMETRY_H

#include <vector>
#include <Eigen/Dense>
#include <cmath>

#include "Patch.h"

using namespace Eigen ;

class Geometry{
public:
    virtual  void set(int x, int y ) = 0;
    virtual Patch getPatch(int degree, int x, int y ) = 0;
};

class Sphere :public Geometry{
private:
    Vector3d center ;
    double radius ;

    int xPatch = 0 ;
    int yPatch = 0 ;
    double stepx = 0.0 ;
    double stepy = 0.0 ;
public:
    Sphere( Vector3d center, double radius)
    :center(center),radius(radius) {}

    void set(int xPatch, int yPatch) {
        this->xPatch = xPatch ;
        this->yPatch = yPatch ;

        stepx = 2.0*M_PI / xPatch ;
        stepy = 2.0*M_PI / yPatch ;
    }


    std::vector<std::vector<Vector3d>> getSurface(int xsize, int ysize)
    {
        std::vector<std::vector<Vector3d>> res ;
        double xstep = 2.0*M_PI/(xsize-1) ;
        double ystep = 2.0*M_PI/(ysize-1) ;

        for(double i = 0.0 ; i <= 2.0*M_PI ;i+=xstep )
        {
            std::vector<Vector3d>  t ;
            for(double j = 0.0; j <= 2.0 * M_PI; j+=ystep)
            {
                Vector3d tt = Vector3d(radius*cos(j)*sin(i),radius*cos(j)*cos(i), radius*sin(j) ) ;
                t.push_back(tt) ;
            }
            res.push_back(t) ;
        }
        return res ;
    }

     Patch getPatch(int degree, int x, int y )
     {
        std::vector<std::vector<Vector3d>> res ;
        for(double xbegin = x*stepx ; xbegin < (x+1)*stepx ; xbegin += (stepx)/degree)
        {
            std::vector<Vector3d>  t ;
            for(int ybegin = y*stepy ; ybegin < (y+1)*stepy ; ybegin += (stepy)/degree)
            {
                Vector3d tt = Vector3d(cos(xbegin)*sin(stepy),cos(xbegin)*cos(stepy), sin(xbegin) ) ;
                t.push_back(tt) ;
            }
            res.push_back(t) ;
        }
        return Patch(degree,res) ;
     }

     double getError(std::vector<Vector3d>& allPoints)
     {
        double res = 0.0 ;
        for(int i = 0 ; i < allPoints.size();i++)
        {
            res += abs((allPoints[i] - center).norm()-radius) ;
        }
        return res ;
     }
};
//
//class Cone: public Geometry{
//private:
//    Vector3d center ;
//    Vector3d dir ;
//    double radius ;
//    double height ;
//
//    int xPatch = 0 ;
//    int yPatch = 0 ;
//    double stepx = 0.0 ;
//    double stepy = 0.0 ;
//
//public:
//    Cone(Vector3d& center, double radius, Vector3d Dir , double  height)
//    :center(center),dir(Dir),radius(radius),height(height){}
//
//    void set(int xPatch, int yPatch) {
//        this->xPatch = xPatch ;
//        this->yPatch = yPatch ;
//
//        stepx = 360.0 / xPatch ;
//        stepy = double(height) / yPatch ;
//    }
//
//    Patch getPatch(int degree, int x, int y )
//    {
//        std::vector<std::vector<Vector3d>> res ;
//        for(double xbegin = x*stepx ; x < (x+1)*stepx ; xbegin += (stepx)/degree)
//        {
//            std::vector<Vector3d>  t ;
//            for(int ybegin = y*stepy ; ybegin < (y+1)*stepy ; ybegin += (stepy)/degree)
//            {
//
//            }
//            res.push_back(t) ;
//        }
//
//    }
//
//};


#endif //BEZIERANDBSPLINE_GEOMETRY_H
