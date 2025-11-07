#include <iostream>
#include "Spline.h"

namespace Geometry
{
    Spline::Spline(SplinePoint startPoint, SplinePoint endPoint)
        : m_startPoint{ startPoint }, m_endPoint{ endPoint }
    {
        Eigen::Matrix3d H{
            {  0.0,  1.0,  0.0},
            {  3.0, -2.0, -1.0},
            { -2.0,  1.0,  1.0},
        };

        Eigen::Matrix3d Y{};
        Y.row(0) = endPoint.point.GetPosition()  - startPoint.point.GetPosition();
        Y.row(1) = startPoint.tangent;
        Y.row(2) = endPoint.tangent;

        m_equationConsts.block<3, 3>(1, 0) = H * Y;
        m_equationConsts.row(0) = startPoint.point.GetPosition();
    }

    Eigen::MatrixXd Spline::Mesh(Eigen::MatrixXd U)
    {
        Eigen::MatrixXd X{ m_equationConsts.transpose() * U };

        std::cout << "meshing...\n";

        return X;
    }
}
