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

std::vector<float> generate_knots_for_interpolationCurve(int degree, int csize)
{

    std::vector<float> res ;
    for (int i=0; i<degree; i++) {
        res.push_back(0.f);
    }
    float kstep = 1.0 / float(csize -degree) ;
    for(int i = 0 ; i <= float(csize -degree); i++)
    {
        res.push_back(i*kstep) ;
    }
    for (int i=0; i<degree ; ++i) {
        res.push_back(1.f + 1e-6f);
    }

    return res ;
}

std::vector<float> generate_knots_for_Unifrom(int degree, int csize)
{
    std::vector<float> res ;
    for(int i = 0 ; i < degree+csize+1; i++)
    {
        res.push_back(i) ;
    }
    return res ;
}


std::vector<std::vector<float>> generate_knots_for_interpolationSurface(int degreex, int csize_x, int degreey, int csize_y )
{
    std::vector<std::vector<float>> data ;
    data.push_back(generate_knots_for_interpolationCurve(degreex,csize_x)) ;
    data.push_back(generate_knots_for_interpolationCurve(degreey, csize_y)) ;
    return data;
}

void generateBezierCurve(std::vector<Vector3d>& cpoints, int Precisions, bool isdeCalsteljau)
{
    bezierCurve bc(&cpoints[0],cpoints.size()) ;
    bc.setPrecisions(Precisions) ;
    Vector3d* res = bc.calCurve(isdeCalsteljau) ;
    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            res ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &cpoints[0],
            Precisions,
            cpoints.size(),
            FLAG_LINE) ;
}

void generateBSplineCurve(bool isCox, std::vector<Vector3d>&cpoints,int degree, int Precision)
{
    std::vector<float> knots = generate_knots_for_interpolationCurve(degree,cpoints.size()) ;
//    std::vector<float> knots = generate_knots_for_Unifrom(degree,cpoints.size()) ;

    Bspline spline(&cpoints[0],knots,degree,cpoints.size()) ;
    spline.setPrecisions(Precision) ;
    std::vector<Vector3d>& res1 = isCox?spline.calCurve_By_Cox_deboor():spline.calCurve_By_deboor()  ;

    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &res1[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &cpoints[0],
            res1.size(),
            cpoints.size(),
            FLAG_LINE) ;
}

void generateBSplineSurface(bool isCox, std::vector<std::vector<Vector3d>>& cpoints,int degree, int Precision)
{
    Vector3d** result = new Vector3d*[cpoints.size()] ;
    std::vector<Vector3d> usingControl ;
    for(int i = 0 ; i < cpoints.size() ;i++)
    {
        result[i] = new Vector3d[cpoints[i].size()] ;
        for(int j = 0 ; j < cpoints[i].size();j++)
        {
            result[i][j] = cpoints[i][j] ;
            usingControl.push_back(cpoints[i][j]) ;
        }
    }



    bSplineSurface bsplinesurface(result,cpoints.size(),cpoints[0].size(),degree) ;
    bsplinesurface.setPrecision(Precision,Precision) ;
    if(isCox)  bsplinesurface.calSurface_By_Cox_deboor(); else  bsplinesurface.calSurface_By_deboor() ;

    std::vector<Vector3d> tris = bsplinesurface.getTriangles() ;
    std::vector<Vector3d> lines = bsplinesurface.getLines() ;

//    bsplinesurface.showCalculatePoints() ;
//    for(int i = 0 ; i < res1.size(); i++)
//    {
//        std::cout<<res1[i].transpose()<<std::endl;
//    }


//    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
//            &tris[0] ,
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
//            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
//            &usingControl[0],
//            tris.size(),
//            usingControl.size(),
//            FLAG_TRIANGLE) ;

    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &lines[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &usingControl[0],
            lines.size(),
            usingControl.size(),
            FLAG_LINE) ;


}


void InterpolationBSplineCurve(bool isCox, std::vector<Vector3d> &dpoints, int degree) {
    std::vector<float> knots = generate_knots_for_interpolationCurve(degree,dpoints.size()) ;
    InterpolationCurve interpolationCurve = InterpolationCurve(degree,knots,dpoints) ;
    std::vector<Vector3d>& res1 = interpolationCurve.getPoints(1000,isCox) ;
    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &res1[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &dpoints[0],
            res1.size(),
            dpoints.size(),
            FLAG_LINE) ;
}

void ApproximationBSplineCurve(bool isCox, std::vector<Vector3d>& dpoints, int degree)
{
    std::vector<float> knots = generate_knots_for_interpolationCurve(degree,dpoints.size()) ;
    ApproximationCurve approximationCurve = ApproximationCurve(2,knots, dpoints) ;
    std::vector<Vector3d>& res2 = approximationCurve.getPoints(1000,isCox) ;

    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &res2[0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &dpoints[0],
            res2.size(),
            dpoints.size(),
            FLAG_LINE) ;
}

void InterpolationBsplineSurface(bool isCox, std::vector<std::vector<Vector3d>>& dpoints, int degreex, int degreey, int Precision,Sphere sp)
{
    std::vector<std::vector<float>> XYKnots = generate_knots_for_interpolationSurface(degreex,dpoints.size(),degreey,dpoints[0].size()) ;
    InterpolationSurface ss = InterpolationSurface(isCox,degreex,XYKnots,dpoints,Precision,Precision) ;
    std::vector<Vector3d> cc = ss.getPoints() ;

    std::cout<<degreex<<" this error is "<<sp.getError(cc)<<std::endl  ;

//    return ;
    for(int i  = 0 ; i < dpoints.size(); i++) {
        for (int j = 0; j < dpoints[0].size(); j++) {
            cc.push_back(dpoints[i][j]);
        }
    }
    display("C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\render.fs",
            &dpoints[0][0] ,
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.vs",
            "C:\\Users\\lidan\\Desktop\\BezierAndBspline-master\\glsl\\point.fs",
            &cc[0],
            0,
            cc.size(),
            FLAG_LINE) ;
}

void testBezierCurve()
{

    std::vector<Vector3d> controlP ;

    controlP.push_back(Vector3d(-1.0,-1.0,-1.0)) ;
    controlP.push_back(Vector3d(-2.0,-3.0,-1.0 )) ;
    controlP.push_back(Vector3d(-3.0,-0.8,-1.0)) ;
    controlP.push_back(Vector3d(-4.0,-1.0,-1.0)) ;

    generateBezierCurve(controlP,1000, true ) ;
}

void testBsplineCurve()
{
    std::vector<Vector3d>  result1(4) ;
    result1[0]<<-1.0,0.0 ,-1.0 ;
    result1[1]<<-0.5,0.5 ,-1.0 ;
    result1[2]<<0.5 ,-0.5,-1.0 ;
    result1[3]<<1.  ,0.0 ,-1.0  ;
    generateBSplineCurve(true, result1,3, 1000) ;

}


void  testGenerateBsplineSurface()
{
    std::vector<std::vector<Vector3d>> results ;
    for(int i  = 0 ; i < 4; i++)
    {
        std::vector<Vector3d> rr ;
        for(int j = 0 ; j < 4;j++)
        {
            rr.push_back(Vector3d(i,j,sqrt(32.0-i*i-j*j)) ) ;
        }
        results.push_back(rr) ;
    }
    generateBSplineSurface(true, results, 2,100) ;
}


void testInterpolationBSplineCurve()
{
//    Sphere sphere =  Sphere(Vector3d(0,0,0),1.0);
//    std::vector<std::vector<Vector3d>> datapoints = sphere.getSurface(10,10) ;
    std::vector<Vector3d>  result1(4) ;
    result1[0]<<-1.0,0.0,-1.0 ;
    result1[1]<<-0.5,  0.5,-1.0 ;
    result1[2]<<0.5, -0.5,-1.0 ;
    result1[3]<<1.0,0.0,-1.0 ;
    InterpolationBSplineCurve(true,result1,2) ;
}


void testApproximationBSplineCurve()
{
    std::vector<Vector3d>  result1(4) ;
    result1[0]<<-1.0,0.0,-1.0 ;
    result1[1]<<-0.5,  0.5,-1.0 ;
    result1[2]<<0.5, -0.5,-1.0 ;
    result1[3]<<1.0,0.0,-1.0 ;
    ApproximationBSplineCurve(true, result1, 2) ;
}

void testInterpolationBsplineSurface()
{
    Sphere sphere =  Sphere(Vector3d(0,0,0),1.0);
    std::vector<std::vector<Vector3d>> datapoints = sphere.getSurface(15,15) ;

//    for(int i = 3; i <15; i++)
//    {
//        InterpolationBsplineSurface(true, datapoints, i,i,500,sphere) ;
//    }
    InterpolationBsplineSurface(true, datapoints, 3,3,500,sphere) ;
}


int main()
{
    testBezierCurve() ;
//    testGenerateBsplineSurface();
//    testBsplineCurve() ;
//    testInterpolationBSplineCurve() ;
//    testApproximationBSplineCurve() ;
//    testInterpolationBsplineSurface() ;
}