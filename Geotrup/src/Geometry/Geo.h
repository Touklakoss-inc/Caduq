#ifndef GO_GEO_H
#define GO_GEO_H

#include <Eigen/Core>

#include "Point.h"

namespace Geometry
{
    typedef Eigen::Transform<double, 3, Eigen::TransformTraits::Affine> Transform;

    struct Mesh
    {
        Eigen::MatrixXd nodes;
        Eigen::VectorX<uint32_t> elts;
    };

    struct SplinePoint
    {
        Point point{};
        Eigen::RowVector3d tangent{ 0.0, 0.0, 0.0 };
        double tension{ 0 };
    };
}

#endif
