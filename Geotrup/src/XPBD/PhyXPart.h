#ifndef PHYXPART_H
#define PHYXPART_H

#include "Geometry/Frame.h"

namespace XPBD
{
    struct PhyXPart
    {
        Geometry::Frame* frame;
        double mass;
        bool isGrounded;
    };
}

#endif
