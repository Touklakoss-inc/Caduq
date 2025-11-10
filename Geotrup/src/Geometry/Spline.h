#ifndef SPLINE_H
#define SPLINE_H

#include <Eigen/Dense>

#include "Point.h"

namespace Geometry
{
    struct SplinePoint
    {
        Geometry::Point point{};
        Eigen::RowVector3d tangent{ 0.0, 0.0, 0.0 };
        double tension{ 0 };
    };

    class Spline
    {
        private:
        SplinePoint m_startPoint{};
        SplinePoint m_endPoint{};

        Eigen::MatrixXd m_equationConsts{ 4, 3 };

        public:
        Spline(SplinePoint startPoint, SplinePoint endPoint);
        Eigen::MatrixXd Mesh(Eigen::ArrayXd u, const int MESH_SIZE);
    };
}

#endif
