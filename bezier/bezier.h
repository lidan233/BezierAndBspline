//
// Created by lidan on 24/10/2020.
//

#ifndef BEZIERANDBSPLINE_BEZIER_H
#define BEZIERANDBSPLINE_BEZIER_H

class bezier{
public:
    int precision ;
    bool showControlPoints ;

    bezier(){
        precision = 5 ; // need how many points
        showControlPoints = true ;
    }

    virtual void draw()
    {}

    void setPrecisions(int precision)
    {
        if(precision < 2)
        {
            precision = 2 ;
        }
        this->precision = precision ;
    }

    void controlPoints(bool flag)
    {
        showControlPoints = flag ;
    }

};

#endif //BEZIERANDBSPLINE_BEZIER_H
