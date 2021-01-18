//
// Created by lidan on 16/1/2021.
//

#include "FitGeometry.h"


InterpolationCurve::InterpolationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points)
:_points(points)
{
    int size = points.size() ;
    N.resize(size,size) ;

    for(int i = 0 ; i < size; i++)
    {
        for(int j = 0 ; j < size ;j++)
        {
            N(i,j) = Bspline::calculate_B_Function(knots,j,degree,knots[degree+i]) ;
        }
    }

     N = N.inverse() ;

    Matrix<double,Dynamic,Dynamic> tt ;
    tt.resize(size,3) ;

    for(int i =0 ; i < size ; i++)
    {
        tt.row(i) = points[i] ;
    }

    Matrix<double,Dynamic,Dynamic> ttt = N * tt ;

    for(int i =0 ; i< size ; i++)
    {
        _controlPoints[i] = ttt.row(i) ;
    }
    bspline = new Bspline(&_controlPoints[0],knots,degree,_controlPoints.size()) ;

}

std::vector<Vector3d>& InterpolationCurve::getPoints(int Precisions)
{
    bspline->setPrecisions(Precisions) ;
    return bspline->calCurve_By_deboor() ;
}
