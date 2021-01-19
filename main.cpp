#include <iostream>
#include <Eigen/Dense>
#include "ui/ui.h"
#include "bezierCurve.h"
#include "bspline.h"
#include "bplineSurface.h"
#include "FitGeometry.h"
#include "Geometry.h"

using namespace std;
using namespace Eigen ;



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




    std::vector<Vector3d>  result1(4) ;
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

// calculate curve
//    Bspline spline(&result1[0],knots,2,4) ;
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
            results[i][j] = Vector3d(i,j,sqrt(32.0-i*i-j*j)) ;
        }
    }


//    bSplineSurface bsplinesurface(results,4,4,2) ;
//    bsplinesurface.setPrecision(100,100) ;
////    bsplinesurface.calSurface_By_Cox_deboor() ;
//    bsplinesurface.calSurface_By_deboor() ;
//    std::vector<Vector3d> tris = bsplinesurface.getTriangles() ;
//    std::vector<Vector3d> lines = bsplinesurface.getLines() ;
//
//    bsplinesurface.showCalculatePoints() ;

////    for(int i = 0 ; i < res1.size(); i++)
////    {
////        std::cout<<res1[i].transpose()<<std::endl;
////    }
//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &tris[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            &result1[0],
//            tris.size(),
//            4,
//            FLAG_TRIANGLE) ;
//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &lines[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            &result1[0],
//            lines.size(),
//            4,
//            FLAG_LINE) ;

    std::vector<float> _knots ;
    for (int i=0; i<2; i++) {
        _knots.push_back(0.f);
    }
    float kstep = 1.0 / (4.0 -2.0) ;
    for(int i = 0 ; i <= 4.0-2.0; i++)
    {
        _knots.push_back(i*kstep) ;
    }
    for (int i=0; i<2; ++i) {
        _knots.push_back(1.f + 1e-6f);
    }


//    InterpolationCurve interpolationCurve = InterpolationCurve(2,_knots,result1) ;
//    std::vector<Vector3d>& res1 = interpolationCurve.getPoints(1000) ;

//    ApproximationCurve approximationCurve = ApproximationCurve(2,_knots, result1) ;
//    std::vector<Vector3d>& res2 = approximationCurve.getPoints(1000) ;
//
//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &res2[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            &result1[0],
//            res2.size(),
//            4,
//            FLAG_LINE) ;


//    std::vector<std::vector<Vector3d>> results11  ;
//    for(int i  = 0 ; i < 4; i++)
//    {
//        results11.push_back(std::vector<Vector3d>()) ;
//        for(int j = 0 ; j < 4;j++)
//        {
//            results11[i].push_back(Vector3d(i,j,sqrt(32.0-i*i-j*j))) ;
//        }
//    }
    Sphere sphere =  Sphere(Vector3d(0,0,0),1.0);
    std::vector<std::vector<Vector3d>> results11 = sphere.getSurface(10,10) ;

    std::vector<float> s_knots ;
    for (int i=0; i<2; i++) {
        s_knots.push_back(0.f);
    }
    float skstep = 1.0 / (10.0-2.0) ;
    for(int i = 0 ; i <= 10.0-2.0; i++)
    {
       s_knots.push_back(i*skstep) ;
    }
    for (int i=0; i<2; ++i) {
        s_knots.push_back(1.f + 1e-6f);
    }


    std::vector<std::vector<float>> XYKnots;
    XYKnots.push_back(s_knots) ;
    XYKnots.push_back(s_knots) ;

    InterpolationSurface ss = InterpolationSurface(2,XYKnots,results11,300,300) ;
    std::vector<Vector3d> cc = ss.getPoints() ;

//    for(int i  = 0 ; i < results11.size(); i++)
//    {
//        for(int j = 0 ; j < results11[0].size();j++)
//        {
//            cc.push_back(results11[i][j]) ;
//        }
//    }



    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &res[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &cc[0],
            0,
            cc.size(),
            FLAG_LINE) ;

    return 0;
}