#ifndef PHYXPART_H
#define PHYXPART_H

#include "Geometry/Frame.h"

#include <memory>
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
