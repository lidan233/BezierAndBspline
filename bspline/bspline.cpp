//
// Created by lidan on 28/10/2020.
//

#include "bspline.h"
Bspline::Bspline(Vector3d* points, std::vector<float> &ks, int degree, int csize)
{
    this->degree = degree ;
    for(int i = 0 ; i < csize ; i++)
    {
        _controlpoints.push_back(Vector3d(points[i][0],points[i][1],points[i][2])) ;
    }

    for(int k : ks)  knots.push_back(k) ;
    auto_produce_knots() ;
    assert(knots.size() == degree+1+csize) ;
    for(int i = 0 ; i < knots.size(); i++) std::cout<<knots[i]<<" " ;
}


void Bspline::setPrecisions(int precisions)
{
    this->Precisions = precisions ;
}

void  Bspline::getCubicPoint(float t,int start, Vector3d& res)  {
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

std::vector<Vector3d>& Bspline::calCubicCurve()
{
    _points.clear() ;
    for(int i = 0 ; i< Precisions ; i++)
    {
        float t = i / Precisions ;
        Vector3d res ;
        getCubicPoint(t,i,res ) ;
//        res[2] = -1 ;
        _points.push_back(res) ;
    }
    return _points ;
}


void Bspline::auto_produce_knots()
{
    int all_Knum = _controlpoints.size() + degree + 1;

    if(knots.size() == 0)
    {
        int inKnot = all_Knum - 2*(degree+1);
        for (int i=0; i<=degree; ++i) {
            knots.push_back(0.f);
        }
        if (inKnot > 0) {
            for (size_t i=1; i<=inKnot; ++i) {
                knots.push_back(i);
            }
        }

        for (int i=0; i<=degree; ++i) {
            knots.push_back(inKnot);
        }
    }else if(knots.size()==_controlpoints.size())
    {
//        std::vector<float> t = knots ;
        knots.clear() ;
        for (int i=0; i<degree; i++) {
            knots.push_back(0.f);
        }

//        int begin = ceil((degree+1)/2.f) - 1;
//        int end = all_Knum - floor((degree+1)/2.f);
//
//        for (int i=begin; i<=end; ++i) {
//            knots.push_back( (t[i]-t[begin])/(t[end]-t[begin]) );
//        }

        float kstep = 1.0f / (_controlpoints.size() - degree) ;
        for(int i = 0 ; i <= _controlpoints.size()-degree;i++)
        {
            knots.push_back(i*kstep) ;
        }

        for (int i=0; i<degree; ++i) {
            knots.push_back(1.f + 1e-6f);
        }

    }else if(knots.size()!=all_Knum){
        knots.clear() ;
        auto_produce_knots() ;
    }
}

 double Bspline::calculate_B_Function(std::vector<float>& knots, int K_begin, int degree, float t )
{
    if (degree == 0) {
        if ( t >= knots[K_begin] && t < knots[K_begin+1] )
            return 1.f;
        else
            return 0.f;
    }else{
        double first, second ;

        if(knots[K_begin+degree] == knots[K_begin])
        {
            first = 0 ;
        } else{
            first = double(t - knots[K_begin]) / double(knots[K_begin+degree] - knots[K_begin]);
        }

        if(knots[K_begin+degree+1] == knots[K_begin+1])
        {
            second = 0 ;
        }else{
            second = double(knots[K_begin+degree+1] - t) / double(knots[K_begin+degree+1] - knots[K_begin+1]);
        }

        if(first == 0.0f)
            return second * calculate_B_Function(knots,K_begin+1, degree-1 , t) ;
        else if(second ==0.0f)
            return first*calculate_B_Function(knots,K_begin,degree-1,t) ;
        else
            return first*calculate_B_Function(knots,K_begin,degree-1,t) +
                second * calculate_B_Function(knots,K_begin+1, degree-1 , t) ;
    }
}


std::vector<Vector3d>& Bspline::calCurve_By_deboor()
{
    float rt = 1.f/(Precisions) ;
    _points.clear() ;
    for(int i = 0 ; i < this->Precisions+1 ;i++)
    {
        _points.push_back(calculate_One_deboor(rt*i)) ;
    }
    return _points ;
}

std::vector<Vector3d>& Bspline::calCurve_By_Cox_deboor()
{
    float rt = 1.f/(Precisions) ;
    _points.clear() ;
    for(int i = 0 ; i < this->Precisions ;i++)
    {
        _points.push_back(calculate_one_cox_deboor(rt*i*(knots[knots.size()-1]-knots[0]))) ;
    }
    return _points ;
}


Vector3d Bspline::calculate_One_deboor(float t)
{
    std::cout<<" for "<<t <<" " ;
    int csize = this->_controlpoints.size() ;
    int k_size = this->knots.size() ;
    int n_points = this->Precisions ;
    int d = this->degree ;
    assert(n_points!=0 && k_size == d+1+csize &&(t>=0||t<=1)) ;

    float begin = knots[d] ;
    float end = knots[k_size - degree -1];
    t =  t * (end - begin) + begin;

//    std::cout<<t<<" " ;
//    std::cout<<begin<<" "<<end<<" " ;

    int index = d ;
    for(; index<k_size - degree -1; index++) {
        if(t >= knots[index] && t <= knots[index+1]) {
            break;
        }
    }

    std::vector<Vector3d> points;
    for(int i =0 ; i < csize ; i++)
    {
        points.push_back(_controlpoints[i]) ;
    }

    std::cout<<index<<" " ;

    for(int i = 1 ; i <= degree+1 ; i++)
    {
        for(int j =  index; j > index-degree-1+i ; j--)
        {
            double alpha = double( t - knots[j]) / double(knots[j+degree+1-i] - knots[j]) ;
//            std::cout<<alpha<<" "<<double( t - knots[j])<<" "<<std::endl ;
            points[j] = (1-alpha) * points[j-1] + alpha * points[j] ;
        }
    }

    std::cout<<points[index].transpose()<<std::endl ;
    return  points[index] ;
}

Vector3d Bspline::calculate_one_cox_deboor(float t)
{
    std::cout<<" for "<<t<<" " ;
    Vector3d res(0.f,0,0);
    for (size_t i=0; i<_controlpoints.size(); ++i)
    {
        double t1 = calculate_B_Function(knots, i, degree, t) ;
        std::cout<<t1<<" " ;
        res += calculate_B_Function(knots, i, degree, t)*_controlpoints.at(i);
    }
    std::cout<<std::endl ;
//    res[2] = -1 ;
    std::cout<<res.transpose()<<std::endl ;
    return res;
}



//MatrixXd Bspline::getPointsByRBasis()
//{
//    int degree = this->degree+1 ;
//    MatrixXd basis = MatrixXd::Zero(degree, degree) ;
//    basis(degree-1,degree-1) = 1.0 ;
////    basis(degree-1,i) = 1.0 ;
//    int count = degree ;
//    int nowlevel = 2 ;
//    for(int k = 0 ; k < degree-1 ; k++)
//    {
//        for(int i = 0 ; i < degree ;i++ )
//        {
//            MatrixXd b1 =  basis.col(i) ;
//            MatrixXd b11 = basis.col(i) ;
//            MatrixXd b2 ;
//            MatrixXd b22 ;
//            if(i+1 >= degree) b2 = (MatrixXd::Zero(degree,1)) ; else  b2 = (basis.col(i+1)) ;
//            if(i+1 >= degree) b22 = (MatrixXd::Zero(degree,1)) ; else  b22 = (basis.col(i+1)) ;
//
//            double i1 = 1.0 / ((knots[i+nowlevel-1] - knots[i])<0.0000001?0.0000001:(knots[i+nowlevel-1] - knots[i])) ;
//            double i2 = i1 * -knots[i] ;
//
//            double i4 = -1.0 / ((knots[i+nowlevel] - knots[i+1])<0.0000001?0.0000001:(knots[i+nowlevel] - knots[i+1])) ;
//            double i3 = -i4 * knots[i + nowlevel] ;
//
////            std::cout<<" xishu shi"<<i1<<" "<<i2<<" "<<i3<<" "<<i4<<std::endl;
////            std::cout<<" xishu shi"<<knots[i+nowlevel]<<" "<<knots[i+1]<<" "<<knots[i+nowlevel+1]<<" "<<knots[i+2]<<std::endl;
//            b1.block(0,0,degree-1,1) = b1.block(1,0,degree-1,1) * i1 ;
//            b1(degree-1,0) = 0.0 ;
//            b11.block(1,0,degree-1,1) =  b11.block(1,0,degree-1,1) * i2 ;
//
//            b2.block(0,0,degree-1,1) = b2.block(1,0,degree-1,1) * i4 ;
//            b2(degree-1,0) = 0.0 ;
//            b22.block(1,0,degree-1,1) =  b22.block(1,0,degree-1,1) * i3 ;
//
////            std::cout<<b1.transpose()<<" end"<<std::endl ;
////            std::cout<<b11.transpose()<<" end"<<std::endl ;
////            std::cout<<b2.transpose()<<" end"<<std::endl ;
////            std::cout<<b22.transpose()<<" end"<<std::endl ;
//
//            basis.col(i) = b1.col(0) +b2.col(0)+b11.col(0)+b22.col(0) ;
//        }
//        std::cout<<"the "<<k<<std::endl ;
//        std::cout<<basis<<std::endl ;
//        count-- ;
//        nowlevel++ ;
//    }
//
//    return basis ;
//}

