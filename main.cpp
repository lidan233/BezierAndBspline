#include <iostream>
#include <Eigen/Dense>
#include "ui/ui.h"
#include "bezierCurve.h"
#include "bspline.h"
#include "bplineSurface.h"


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
//            res ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            result,
//            100,
//            4,
//            FLAG_LINE) ;




    Vector3d* result1 = new Vector3d[4] ;
    result1[0]<<-1.0,0.0,-1.0 ;
    result1[1]<<-0.5,  0.5,-1.0 ;
    result1[2]<<0.5, -0.5,-1.0 ;
    result1[3]<<1.0,0.0,-1.0 ;


//    result1[3]<<-4.0,-1.0,-1.0 ;


    std::vector<float> knots ;
//    knots.push_back(0) ;
//    knots.push_back(1) ;
//    knots.push_back(2) ;
//    knots.push_back(3) ;
//    knots.push_back(4) ;
//    knots.push_back(5) ;
//    knots.push_back(6) ;

    knots.push_back(0) ;
    knots.push_back(0) ;
    knots.push_back(0) ;
    knots.push_back(1) ;
    knots.push_back(2) ;
    knots.push_back(2) ;
    knots.push_back(2) ;


//    Bspline spline(result1,knots,2,4) ;
//    spline.setPrecisions(1000) ;
//    std::vector<Vector3d>& res1 = spline.calCurve_By_Cox_deboor() ;

//    std::vector<Vector3d>& res1 = spline.calCurve_By_deboor() ;

//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &res1[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            result1,
//            res1.size(),
//            4,
//            FLAG_LINE) ;



//    // calculate surface
    Vector3d** results = new Vector3d*[4] ;
    for(int i  = 0 ; i < 4; i++)
    {
        results[i] = new Vector3d[4] ;
        for(int j = 0 ; j < 4;j++)
        {
            results[i][j] = Vector3d(i,j,sqrt(32.0-i*i-j*j));
        }
    }
    bSplineSurface bsplinesurface(results,4,4,2) ;
    bsplinesurface.setPrecision(100,100) ;
//    bsplinesurface.calSurface_By_Cox_deboor() ;
    bsplinesurface.calSurface_By_deboor() ;
    std::vector<Vector3d> tris = bsplinesurface.getTriangles() ;
    std::vector<Vector3d> lines = bsplinesurface.getLines() ;

////    for(int i = 0 ; i < res1.size(); i++)
////    {
////        std::cout<<res1[i].transpose()<<std::endl;
////    }
//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &tris[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            result1,
//            tris.size(),
//            4,
//            FLAG_TRIANGLE) ;
    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &lines[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            result1,
            lines.size(),
            4,
            FLAG_LINE) ;

    return 0;
}