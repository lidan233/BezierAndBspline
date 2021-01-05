//
// Created by lidan on 28/10/2020.
//

#include "bspline.h"

Bspline::Bspline(Vector3d* points, std::vector<int> &ks, int degree, int csize)
{
    this->degree = degree ;

    for(int i = 0 ; i < csize ; i++)
    {
        _controlpoints.push_back(Vector3d(points[i][0],points[i][1],points[i][2])) ;
    }

    for(int i = 0 ; i < degree ; i++) knots.push_back(0) ;
    for(int k : ks)  knots.push_back(k) ;
    for(int i = 0 ; i < degree; i++) knots.push_back(ks[ks.size()-1]) ;
}

// three degree
void  Bspline::getPoint(float t,int start, Vector3d& res)  {
    MatrixXd bsplineR(4,4) ;
    MatrixXd controlpoints(4,2) ;
    MatrixXd points(1,4) ;

    bsplineR<<-1, 3, -3, 1,
                 3, -6, 3, 0,
            -3, 0, 3, 0,
            1, 4, 1, 0;

    points<<pow(t,3) , pow(t,2), pow(t,1) ,pow(t,0) ;

    printf("%f",points(0,1));
    auto startp = this->_controlpoints.begin() ;
    std::advance(startp,start) ;


    for(int i = 0 ; i< 4;i++)
    {
        controlpoints<<(*startp)[0];
        controlpoints<< (*startp)[1];

        startp = startp++ ;
    }

    MatrixXd r =  points*bsplineR*controlpoints ;
    res[0] = r(0,0) ;
    res[1] = r(0,1) ;
}

void Bspline::getpoints(int num)
{
    _points.clear() ;
    for(int i = 0 ; i< num-4 ; i++)
    {
        float t = i / num ;
        Vector3d res ;
        getPoint(t,i,res ) ;
        res[2] = -1 ;
        _points.push_back(res) ;
    }
}

/*
 *  std::list<Vector2d> _controlpoints ;
    std::vector<Vector2d> _points ;
    std::vector<float> knots ;
    int degree ;
    points * bsplineR * controlpoints ;
    using this param to calculate basis
 */
Vector2d Bspline::getPointsByRPoint(int k_point_begin, float t )
{
    // the begining basis is the Identity matrix
    std::vector<Vector2d> tt ;
    for(int i = 0 ; i <= degree ; i++)
    {
        tt.push_back(Vector2d(_controlpoints[i+k_point_begin-degree][0],_controlpoints[i+k_point_begin-degree][1])) ;
    }

    for(int i = 1 ; i < degree +1 ; i++)
    {
        for(int j = degree ; j > i -1 ; j--)
        {
            float alpha = (t - knots[j+k_point_begin-degree]) / (knots[j+k_point_begin-i+1] - knots[j+k_point_begin-degree]) ;
            tt[j] = (1.0 - alpha) * tt[j-1] + alpha * tt[j] ;
        }
    }
    return tt[degree] ;
}

MatrixXd Bspline::getPointsByRBasis()
{
    int degree = this->degree+1 ;
    MatrixXd basis = MatrixXd::Zero(degree, degree) ;
    basis(degree-1,degree-1) = 1.0 ;

    int count = degree ;
    int nowlevel = 2 ;
    for(int k = 0 ; k < degree-1 ; k++)
    {
        for(int i = count-2 ; i < degree ;i++ )
        {
            MatrixXd b1 =  basis.col(i) ;
            MatrixXd b11 = basis.col(i) ;
            MatrixXd b2 ;
            MatrixXd b22 ;
            if(i+1 >= degree) b2 = (MatrixXd::Zero(degree,1)) ; else  b2 = (basis.col(i+1)) ;
            if(i+1 >= degree) b22 = (MatrixXd::Zero(degree,1)) ; else  b22 = (basis.col(i+1)) ;

            double i1 = 1.0 / ((knots[i+nowlevel-1] - knots[i])<0.0000001?0.0000001:(knots[i+nowlevel-1] - knots[i])) ;
            double i2 = i1 * -knots[i] ;

            double i4 = -1.0 / ((knots[i+nowlevel] - knots[i+1])<0.0000001?0.0000001:(knots[i+nowlevel] - knots[i+1])) ;
            double i3 = -i4 * knots[i + nowlevel] ;

            std::cout<<" xishu shi"<<i1<<" "<<i2<<" "<<i3<<" "<<i4<<std::endl;
            std::cout<<" xishu shi"<<knots[i+nowlevel]<<" "<<knots[i+1]<<" "<<knots[i+nowlevel+1]<<" "<<knots[i+2]<<std::endl;
            b1.block(0,0,degree-1,1) = b1.block(1,0,degree-1,1) * i1 ;
            b1(degree-1,0) = 0.0 ;
            b11.block(1,0,degree-1,1) =  b11.block(1,0,degree-1,1) * i2 ;

            b2.block(0,0,degree-1,1) = b2.block(1,0,degree-1,1) * i4 ;
            b2(degree-1,0) = 0.0 ;
            b22.block(1,0,degree-1,1) =  b22.block(1,0,degree-1,1) * i3 ;

            std::cout<<b1.transpose()<<" end"<<std::endl ;
            std::cout<<b11.transpose()<<" end"<<std::endl ;
            std::cout<<b2.transpose()<<" end"<<std::endl ;
            std::cout<<b22.transpose()<<" end"<<std::endl ;

            basis.col(i) = b1.col(0) +b2.col(0)+b11.col(0)+b22.col(0) ;
        }
        std::cout<<"the "<<k<<std::endl ;
        std::cout<<basis<<std::endl ;
        count-- ;
        nowlevel++ ;
    }

    return basis ;
}

