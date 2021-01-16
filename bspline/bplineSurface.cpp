//
// Created by lidan on 29/10/2020.
//

#include "bplineSurface.h"

bSplineSurface::bSplineSurface(Vector3d** controlPoints,const unsigned int xsize , const unsigned int ysize ,int degree)
{
    this->_controlPoints = controlPoints ;
    this->xsize = xsize ;
    this->ysize = ysize ;
    this->degree = degree ;
}

void bSplineSurface::getCubicPoint(float tx,float ty,int xorder,int yorder,Vector3d& res)
{
    Vector3d* y = new Vector3d[degree] ;
    for(int i = 0 ; i< degree+1 ;i++)
    {
        getCubicYPoint(ty,xorder,yorder,y[i]) ;
    }

    MatrixXd bsplineR(4,4) ;
    MatrixXd controlpoints(4,3) ;
    MatrixXd points(1,4) ;

    bsplineR<<-1, 3, -3, 1,
            3, -6, 3, 0,
            -3, 0, 3, 0,
            1, 4, 1, 0;

    points<<pow(tx,3) , pow(tx,2), pow(tx,1) ,pow(tx,0) ;

    for(int i = 0 ; i< degree;i++)
    {
        controlpoints<<y[i][0] /6 ;
        controlpoints<<y[i][1] /6 ;
        controlpoints<<y[i][2] /6 ;
    }

    MatrixXd r =  points*bsplineR*controlpoints ;
    res[0] = r(0,0) ;
    res[1] = r(0,1) ;
    res[2] = r(0,2) ;

    delete y ;
}
void bSplineSurface::getCubicYPoint(float ty,int xorder,int yorder, Vector3d& res)
{
    MatrixXd bsplineR(4,4) ;
    MatrixXd controlpoints(4,3) ;
    MatrixXd points(1,4) ;

    bsplineR<<-1, 3, -3, 1,
            3, -6, 3, 0,
            -3, 0, 3, 0,
            1, 4, 1, 0;

    points<<pow(ty,3) , pow(ty,2), pow(ty,1) ,pow(ty,0) ;

    for(int i = 0 ; i< 4;i++)
    {
        controlpoints<<this->_controlPoints[xorder][yorder+i][0] / 6;
        controlpoints<<this->_controlPoints[xorder][yorder+i][1] / 6;
        controlpoints<<this->_controlPoints[xorder][yorder+i][2] / 6;

    }

    MatrixXd r =  points*bsplineR*controlpoints ;
    res[0] = r(0,0) ;
    res[1] = r(0,1) ;
    res[1] = r(0,2) ;
}

void bSplineSurface::setPrecision(int precisionX, int precisionY)
{
    PrecisionSize_Y = precisionY ;
    PrecisionSize_X = precisionX ;
}


void bSplineSurface::getCubicPoints( )
{
    _points = new Vector3d*[xsize/degree*PrecisionSize_X] ;
    for( int i = 0 ; i< xsize/degree*PrecisionSize_X;i++)
    {
        _points[i] = new Vector3d[ysize/degree*PrecisionSize_Y] ;
    }

    for(int i = 0 ; i< xsize ;i+= degree )
    {
        for(int j = 0 ;j<ysize;j+= degree)
        {
            for(int m =0 ; m< PrecisionSize_X ;m++)
            {
                float tx = m / PrecisionSize_X ;
                for( int n = 0 ; n < PrecisionSize_Y ; n++)
                {
                    float ty = m / PrecisionSize_Y ;
                    getCubicPoint(tx,ty,i,j,_points[(i/degree-1)*PrecisionSize_X+m][(j/degree-1)*PrecisionSize_Y+n]) ;
                }
            }
        }
    }
}

Vector3d** bSplineSurface::calSurface_By_deboor()
{
    assert(PrecisionSize_X!=0 && PrecisionSize_Y!=0 ) ;
    if(_points!= nullptr) delete _points ;
    _points = new Vector3d*[PrecisionSize_X] ;
    std::vector<std::vector<Vector3d>> temp ;

    for(int i = 0 ; i < ysize; i++)
    {
        std::vector<Vector3d> t = calculate_Y_curve_By_deboor(i) ;
        temp.push_back(t) ;
    }

    for(int i = 0 ; i < PrecisionSize_X; i++)
    {
        std::vector<Vector3d> t  ;
        for(int j = 0 ; j < temp.size() ; j++)
        {
            t.push_back(temp[j][i]) ;
        }


        // now we get ysize controlPoints ;
        float kstep = 1.0f / ( t.size()-1) ;
        std::vector<float> knots ;
        for(int k = 0 ; k < t.size() ;k++)
        {
            knots.push_back(k*kstep) ;
        }

        Bspline bspline = Bspline(&t[0],knots,degree,t.size()) ;
        bspline.setPrecisions(PrecisionSize_Y) ;
        std::vector<Vector3d>& res = bspline.calCurve_By_deboor() ;

        _points[i] = new Vector3d[res.size()] ;
        for(int k =0 ; k < res.size(); k++)
        {
            _points[i][k] = res[k] ;
        }
    }

    return _points ;
}

std::vector<Vector3d> bSplineSurface::calculate_Y_curve_By_deboor(int index)
{
    std::vector<Vector3d> ycontrols ;
    for(int i = 0 ; i < xsize; i++)
    {
        ycontrols.push_back(_controlPoints[i][index]) ;
    }

    float kstep = 1.0f / ( ycontrols.size()-1) ;
    std::vector<float> knots ;
    for(int i = 0 ; i < ycontrols.size() ;i++)
    {
        knots.push_back(i*kstep) ;
    }

    Bspline bspline = Bspline(&ycontrols[0],knots,degree,ycontrols.size()) ;
    bspline.setPrecisions(PrecisionSize_X) ;
    std::vector<Vector3d>& t = bspline.calCurve_By_deboor() ;
    return t ;
}


Vector3d** bSplineSurface::calSurface_By_Cox_deboor()
{
    assert(PrecisionSize_X!=0 && PrecisionSize_Y!=0 ) ;

    if(_points!= nullptr) delete _points ;
    _points = new Vector3d*[PrecisionSize_X] ;
    std::vector<std::vector<Vector3d>> temp ;

    for(int i = 0 ; i < ysize; i++)
    {
        std::vector<Vector3d> t = calculate_Y_curve_By_Cox_deboor(i) ;
        temp.push_back(t) ;
    }

    for(int i = 0 ; i < PrecisionSize_X; i++)
    {
        std::vector<Vector3d> t  ;
        for(int j = 0 ; j < temp.size() ; j++)
        {
            t.push_back(temp[j][i]) ;
        }

        // now we get ysize controlPoints ;
        float kstep = 1.0f / ( t.size()-1) ;
        std::vector<float> knots ;
        for(int k = 0 ; k < t.size() ;k++)
        {
            knots.push_back(k*kstep) ;
        }

        Bspline bspline = Bspline(&t[0],knots,degree,t.size()) ;
        bspline.setPrecisions(PrecisionSize_Y) ;
        std::vector<Vector3d> res = bspline.calCurve_By_Cox_deboor() ;

        _points[i] = new Vector3d[res.size()] ;
        for(int k =0 ; k < res.size(); k++)
        {
            _points[i][k] = res[k] ;
        }
    }

    return _points ;
}

std::vector<Vector3d> bSplineSurface::calculate_Y_curve_By_Cox_deboor(int index)
{
    std::vector<Vector3d> ycontrols ;
    for(int i = 0 ; i < xsize; i++)
    {
        ycontrols.push_back(_controlPoints[i][index]) ;
    }

    float kstep = 1.0f / ( ycontrols.size()-1) ;
    std::vector<float> knots ;
    for(int i = 0 ; i < ycontrols.size() ;i++)
    {
        knots.push_back(i*kstep) ;
    }

    Bspline bspline = Bspline(&ycontrols[0],knots,degree,ycontrols.size()) ;
    bspline.setPrecisions(PrecisionSize_X) ;
    std::vector<Vector3d> t = bspline.calCurve_By_Cox_deboor() ;
    return t ;
}

std::vector<Vector3d> bSplineSurface::getTriangles()
{
    std::vector<Vector3d> triangles ;

    for(int i = 0 ; i < PrecisionSize_X-1; i++)
    {
        for(int j = 0 ; j < PrecisionSize_Y-1; j++)
        {
            triangles.push_back(( _points[i][j] )) ;
            triangles.push_back(( _points[i+1][j] )) ;
            triangles.push_back(( _points[i][j+1] )) ;

            triangles.push_back((_points[i+1][j])) ;
            triangles.push_back((_points[i][j+1])) ;
            triangles.push_back((_points[i+1][j+1])) ;
        }
    }

    return triangles ;
}


std::vector<Vector3d> bSplineSurface::getLines()
{
    std::vector<Vector3d> lines ;

    for(int i = 0 ; i < PrecisionSize_X-1; i++)
    {
        for(int j = 0 ; j < PrecisionSize_Y-1; j++)
        {
            lines.push_back(( _points[i][j] )) ;
            lines.push_back(( _points[i+1][j] )) ;
            lines.push_back(( _points[i+1][j] )) ;
            lines.push_back(( _points[i][j+1] )) ;
            lines.push_back(( _points[i][j+1] )) ;
            lines.push_back(( _points[i][j] )) ;

            lines.push_back((_points[i+1][j])) ;
            lines.push_back((_points[i][j+1])) ;
            lines.push_back((_points[i][j+1])) ;
            lines.push_back((_points[i+1][j+1])) ;
            lines.push_back((_points[i+1][j+1])) ;
            lines.push_back((_points[i+1][j])) ;
        }
    }

    return lines ;
}