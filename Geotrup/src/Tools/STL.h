#ifndef STL_H
#define STL_H

#include "Geometry/Geo.h"

#include <Eigen/Dense>
#include <string>

namespace Tools
{
    Geometry::Mesh ImportSTL(const std::string& filename);
}

#endif
