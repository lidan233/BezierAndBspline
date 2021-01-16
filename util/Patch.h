//
// Created by lidan on 16/1/2021.
//

#ifndef BEZIERANDBSPLINE_PATCH_H
#define BEZIERANDBSPLINE_PATCH_H

#include <vector>
#include <Eigen/Dense>

using namespace Eigen ;

class Patch {
    Patch(int degree, std::vector<std::vector<Vector3d>> cpoints ) ;
};


#endif //BEZIERANDBSPLINE_PATCH_H
