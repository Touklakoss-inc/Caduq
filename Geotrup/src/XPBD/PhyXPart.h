#ifndef PHYXPART_H
#define PHYXPART_H

#include "Geometry/Frame.h"
#include <Eigen/Dense>

namespace XPBD
{
    struct PhyXPart
    {
        Geometry::Frame* frame;
        double mass;
        Eigen::Vector3d inertiaTensor;
        bool isGrounded;
    };
}

#endif
