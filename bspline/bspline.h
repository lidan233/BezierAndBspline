//
// Created by lidan on 28/10/2020.
//

#ifndef BEZIERANDBSPLINE_BSPLINE_H
#define BEZIERANDBSPLINE_BSPLINE_H

#include <list>
#include <vector>
#include <Eigen/Dense>
#include <iostream>
using namespace Eigen;


class Bspline {
private:
    std::vector<Vector3d> _controlpoints ;
    std::vector<Vector3d> _points ;
    std::vector<float> knots ;
    int degree ;
    int Precisions = 0 ;

    void  getCubicPoint(float t,int order, Vector3d& res) ;
    Vector3d calculate_One_deboor(float t) ;
    Vector3d calculate_one_cox_deboor(float t) ;
    void auto_produce_knots() ;


public:
    Bspline(Vector3d* points, std::vector<float> &ks, int degree, int csize) ;
    void setPrecisions(int precisions) ;
    std::vector<Vector3d>& calCubicCurve() ;
    static double calculate_B_Function(std::vector<float>& knots, int K_begin, int degree, float t ) ;
    std::vector<Vector3d>& calCurve_By_deboor() ;
    std::vector<Vector3d>& calCurve_By_Cox_deboor() ;



};


#endif //BEZIERANDBSPLINE_BSPLINE_H
