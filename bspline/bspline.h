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
public:
    Bspline(Vector3d* points, std::vector<int> &ks, int degree, int csize) ;
    void  getPoint(float t,int order, Vector3d& res) ;
    void getpoints(int num) ;
    Vector2d getPointsByRPoint(int k_point_begin, float t ) ;
    MatrixXd getPointsByRBasis() ;
};


#endif //BEZIERANDBSPLINE_BSPLINE_H
