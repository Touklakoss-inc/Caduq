#include <iostream>
#include "Spline.h"

#include "Eigen/Core"
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

    Eigen::MatrixXd Spline::Mesh(Eigen::ArrayXd u, const int MESH_SIZE)
    {
        Eigen::MatrixXd U{ 4, MESH_SIZE };
        u = Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0);

        U.row(0) = Eigen::VectorXd::Ones(MESH_SIZE);
        U.row(1) = u;
        U.row(2) = u*u;
        U.row(3) = u*u*u;

        std::cout << "meshing...\n";

        return m_equationConsts.transpose() * U;
    }
}
