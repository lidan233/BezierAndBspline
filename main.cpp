#include <iostream>
#include <Eigen/Dense>
#include "ui/ui.h"
#include "bezierCurve.h"
#include "bspline.h"

using namespace std;
using namespace Eigen ;

//int main() {
//
//    std::cout << "Hello, World!" << std::endl;
//    MatrixXd points(1,4) ;
//    float t = 0.1 ;
//    points<<pow(t,3) , pow(t,2), pow(t,1) ,pow(t,0) ;
//    std::cout<<points(0,3) ;
//    return 0;
//}


int main() {
    Vector3d* result = new Vector3d[4] ;

    result[0]<<-1.0,-1.0,-1.0 ;
    result[1]<<-2.0,-3.0,-1.0 ;
    result[2]<<-3.0,-0.8,-1.0 ;
    result[3]<<-4.0,-1.0,-1.0 ;

    bezierCurve bc(result,4) ;
    bc.setPrecisions(100) ;
    Vector3d* res = bc.calCurve() ;


//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            res ,100,FLAG_LINE) ;

    Vector3d* result1 = new Vector3d[4] ;
    result1[0]<<-1.0,-1.0,-1.0 ;
    result1[1]<<-2.0,-3.0,-1.0 ;
    result1[2]<<-3.0,-0.8,-1.0 ;
    result1[3]<<-4.0,-1.0,-1.0 ;
    std::vector<int> knots ;
    knots.push_back(0) ;
    knots.push_back(1) ;
    knots.push_back(2) ;
    knots.push_back(3) ;



    Bspline spline(result1,knots,3,4) ;
    MatrixXd t = spline.getPointsByRBasis() ;
    std::cout<<t<<std::endl ;
    return 0;
}