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
        // Tangent vector are normalized and will be linked with the distance bitween the start and the end point
        Y.row(1) = startPoint.tangent.normalized() * startPoint.tension * GetLength();
        Y.row(2) = endPoint.tangent.normalized() * endPoint.tension * GetLength();
        m_equationConsts.block<3, 3>(1, 0) = H * Y;
        m_equationConsts.row(0) = startPoint.point.GetPosition();
    }

    Eigen::MatrixXd Spline::Mesh(Eigen::ArrayXd u, const int MESH_SIZE)
    {
        Eigen::MatrixXd U{ 4, MESH_SIZE };

        U.row(0) = Eigen::VectorXd::Ones(MESH_SIZE);
        U.row(1) = u;
        U.row(2) = u*u;
        U.row(3) = u*u*u;

        std::cout << "spline meshing...\n";
        m_mesh = m_equationConsts.transpose() * U;

        return m_equationConsts.transpose() * U;
    }

    std::tuple<Eigen::MatrixXd, Eigen::VectorXi> Spline::GetFemMesh()
    {
        Eigen::MatrixXd nodes{ m_mesh };
        Eigen::VectorXi elts{ nodes.cols()*2+1 };

        for (int i = 0; i < nodes.cols(); i++) 
        {
            elts(i*2) = i;
            elts(i*2+1) = i+1;
        }

        return { nodes, elts };
    };


    double Spline::GetLength()
    {
        Eigen::Vector3d l{ m_endPoint.point.GetPosition() - m_startPoint.point.GetPosition() };

        return l.norm();
    }
}
