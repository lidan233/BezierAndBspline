//
// Created by lidan on 29/10/2020.
//

#include "bplineSurface.h"

bSplineSurface::bSplineSurface(Vector2d** controlPoints,const unsigned int xsize , const unsigned int ysize ,int degree)
{
    this->_controlPoints = controlPoints ;
    this->xsize = xsize ;
    this->ysize = ysize ;
    this->degree = degree ;
}

void bSplineSurface::getPoint(float tx,float ty,int xorder,int yorder,Vector2d& res)
{
    Vector2d* y = new Vector2d[degree] ;
    for(int i = 0 ; i< degree ;i++)
    {
        getYPoint(ty,xorder,yorder,y[i]) ;
    }

    MatrixXd bsplineR(4,4) ;
    MatrixXd controlpoints(4,2) ;
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

    }

    MatrixXd r =  points*bsplineR*controlpoints ;
    res[0] = r(0,0) ;
    res[1] = r(0,1) ;

    delete y ;
}
void bSplineSurface::getYPoint(float ty,int xorder,int yorder, Vector2d& res)
{
    MatrixXd bsplineR(4,4) ;
    MatrixXd controlpoints(4,2) ;
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

    }

    MatrixXd r =  points*bsplineR*controlpoints ;
    res[0] = r(0,0) ;
    res[1] = r(0,1) ;
}

void bSplineSurface::getPoints(int precisionx,int precisiony )
{
    _points = new Vector2d*[xsize/degree*precisionx] ;
    for( int i = 0 ; i< xsize/degree*precisionx;i++)
    {
        _points[i] = new Vector2d[ysize/degree*precisiony] ;
    }

    for(int i = 0 ; i< xsize ;i+= degree )
    {
        for(int j = 0 ;j<ysize;j+= degree)
        {
            for(int m =0 ; m< precisionx ;m++)
            {
                float tx = m / precisionx ;
                for( int n = 0 ; n < precisiony ; n++)
                {
                    float ty = m / precisiony ;
                    getPoint(tx,ty,i,j,_points[(i/degree-1)*precisionx+m][(j/degree-1)*precisiony+n]) ;
                }
            }
        }
    }
}

void bSplineSurface::getPointsByRPoint(int num)
{

}


void bSplineSurface::getPointsByRBasis(int num)
{

}




