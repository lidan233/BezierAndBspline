//
// Created by lidan on 16/1/2021.
//

#include "FitGeometry.h"


InterpolationCurve::InterpolationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points)
:_points(points)
{
    int size = points.size() ;
    N.resize(size,size) ;

    double tv = 1.0 / (size-1) ;

    for(int i = 0 ; i < size; i++)
    {
        for(int j = 0 ; j < size ;j++)
        {
            N(i,j) = Bspline::calculate_B_Function(knots,j,degree,tv*i) ;
//            std::cout<<" calculate i "<<i <<" j "<<j<<" "<<N(i,j)<<" t "<<knots[degree+i]<<std::endl ;
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
        Vector3d tttt = ttt.row(i) ;
        _controlPoints.push_back(tttt) ;
//        std::cout<<" new control points "<<tttt.transpose() <<std::endl ;
    }
    bspline = new Bspline(&_controlPoints[0],knots,degree,_controlPoints.size()) ;
}



std::vector<Vector3d>& InterpolationCurve::getPoints(int Precisions, bool isCox)
{
    bspline->setPrecisions(Precisions) ;
    if(isCox) return bspline->calCurve_By_Cox_deboor() ; else return bspline->calCurve_By_deboor() ;
}



Vector3d getQValue(std::vector<Vector3d>& datapoints, std::vector<float>& knots,float tk,int i,int degree )
{
    // 第一个空指点和最后一个控制点是datapoints的里面的值 跟它相同
    Vector3d result = datapoints[i] - Bspline::calculate_B_Function(knots,0,degree,tk) * datapoints[0]
                                - Bspline::calculate_B_Function(knots,datapoints.size()-1,degree,tk)* datapoints[datapoints.size()-1] ;
    return result ;
}


ApproximationCurve::ApproximationCurve(int degree, std::vector<float>& knots, std::vector<Vector3d>& points)
:_points(points)
{
    int size = points.size() ;
    N.resize(size-2,size-2) ;

    std::vector<Vector3d> Qs ;


    double tv = 1.0 / (size-1) ;
    for(int i = 1 ; i < size-1; i++)
    {
        for(int j = 1 ; j < size-1 ;j++)
        {
            N(i-1,j-1) = Bspline::calculate_B_Function(knots,j,degree,tv*i) ;
        }
    }


    // calculate Q functions for data
    for(int i =0 ; i < size -2 ; i++)
    {
        Qs.push_back(getQValue(points, knots,(i+1) *tv,i+1,degree )) ;
    }

    Matrix<double,Dynamic,Dynamic> tt ;
    tt.resize(size-2,3) ;

    for(int i = 0 ; i < size-2 ; i++)
    {
        Vector3d t_Q = Vector3d(0,0,0);
        for(int j = 0 ; j < size -2 ; j++)
        {
            t_Q += N(j,i) * Qs[j] ;
        }
        tt.row(i) = t_Q ;
    }

//    std::cout<<"begin N "<<std::endl ;
//    std::cout<<N<<std::endl ;

    N = N.transpose() * N ;
    N = N.inverse() ;

//    std::cout<<"end N"<<std::endl ;
//    std::cout<<N<<std::endl ;



    Matrix<double,Dynamic,Dynamic> ttt = N * tt ;

    _controlPoints.push_back(points[0]) ;
//    std::cout<<"control points 0 "<<points[0].transpose()<<std::endl ;

    for(int i =0 ; i< size-2 ; i++)
    {
        Vector3d tttt = ttt.row(i) ;
        _controlPoints.push_back(tttt) ;
//        std::cout<<"control points "<<i<<" "<< points[0].transpose()<<std::endl ;
    }
    _controlPoints.push_back(points[points.size()-1]) ;
//    std::cout<<"control points "<<points.size()-1<<" "<< points[0].transpose()<<std::endl ;

    bspline = new Bspline(&_controlPoints[0],knots,degree,_controlPoints.size()) ;

}
std::vector<Vector3d>& ApproximationCurve::getPoints(int Precisions, bool isCox)
{
    bspline->setPrecisions(Precisions) ;
    if(isCox) return bspline->calCurve_By_Cox_deboor() ; else return bspline->calCurve_By_deboor() ;
}

InterpolationSurface::InterpolationSurface(bool isCox, int degreeall, std::vector<std::vector<float>>& knots, std::vector<std::vector<Vector3d>>& points,int PrecisionX,int PrecisionY)
{
    this->degreeall = degreeall ;
    this->_points = points ;
    std::vector<std::vector<Vector3d>> newpoints ;

    for(int i = 0 ; i < points[0].size() ; i++)
    {
        // 0 means knots of a col
        // 1 means knots of a row
        std::vector<float>& kk  = knots[0];
        std::vector<Vector3d> pp  ;
        for(int j = 0 ; j < points.size(); j++)
        {
            pp.push_back(points[j][i]) ;
        }
        InterpolationCurve a  = InterpolationCurve(degreeall,kk,pp) ;
        std::vector<Vector3d>& ttt = a.getPoints(PrecisionX,isCox) ;
        newpoints.push_back( ttt ) ;
    }

    for(int i = 0 ; i < newpoints[0].size() ; i++)
    {
        std::vector<float>& kk1 = knots[1] ;
        std::vector<Vector3d> pp  ;
        for(int j = 0 ; j < newpoints.size(); j++)
        {
            pp.push_back(newpoints[j][i]) ;
        }
        InterpolationCurve a = InterpolationCurve(degreeall,kk1,pp) ;
        std::vector<Vector3d>& ttt = a.getPoints(PrecisionY,isCox) ;
        _controlPoints.push_back( ttt ) ;
    }
}

//std::vector<Vector3d> InterpolationSurface::getTriangles(bool isCox)
//{
//
//}
//
//std::vector<Vector3d> InterpolationSurface::getLines(bool isCox)
//{
//
//}