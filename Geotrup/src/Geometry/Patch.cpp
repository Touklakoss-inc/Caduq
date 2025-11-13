#include "Patch.h"
#include "Eigen/Core"
#include <iostream>

namespace Geometry
{
    Patch::Patch(Geometry::Spline s0, Geometry::Spline s1, Geometry::Spline s2, Geometry::Spline s3)
        : m_s0{ s0 }, m_s1{ s1 }, m_s2{ s2 }, m_s3{ s3 }
    {
    };
    Eigen::VectorXd Patch::F1(Eigen::VectorXd t)
    {
        return (10*t.array().pow(3) - 15*t.array().pow(4) + 6*t.array().pow(5)).matrix();
    }; 
    Eigen::VectorXd Patch::F0(Eigen::VectorXd t)
    {
        return (1 - F1(t).array()).matrix();
    };

    Eigen::MatrixXd Patch::Mesh(Eigen::VectorXd u, Eigen::VectorXd w, const int MESH_SIZE)
    {
        std::cout << "patch meshing...\n";
        // First interpolation construction (Lc)
        Eigen::MatrixXd Lc{ MESH_SIZE*3, MESH_SIZE };
        Eigen::MatrixXd U{ MESH_SIZE*3, 2 };

        Eigen::MatrixXd u0{ m_s0.Mesh(w, MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd u1{ m_s1.Mesh(w, MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        U.col(0) = u0;
        U.col(1) = u1;

        Eigen::MatrixXd Fu{ 2, MESH_SIZE };
        Fu.row(0) = F0(u);
        Fu.row(1) = F1(u);
        Lc = U * Fu;

        // Second interpolation construction (Ld)
        Eigen::MatrixXd Ld{ MESH_SIZE, MESH_SIZE*3 };
        Eigen::MatrixXd w0{ m_s2.Mesh(u, MESH_SIZE).reshaped<Eigen::RowMajor>(MESH_SIZE*3, 1) };
        Eigen::MatrixXd w1{ m_s3.Mesh(u, MESH_SIZE).reshaped<Eigen::RowMajor>(MESH_SIZE*3, 1) };
        U.col(0) = w0;
        U.col(1) = w1;

        Eigen::MatrixXd Fw{ 2, MESH_SIZE };
        Fw.row(0) = F0(w);
        Fw.row(1) = F1(w);
        Ld = Fw.transpose() * U.transpose();

        // Third interpolation, point interpolation
        Eigen::MatrixXd b{ MESH_SIZE*3, MESH_SIZE };
        Eigen::MatrixXd oo{ m_s0.Mesh(Eigen::ArrayXXd::Zero(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd oi{ m_s0.Mesh(Eigen::ArrayXXd::Ones(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd io{ m_s1.Mesh(Eigen::ArrayXXd::Zero(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd ii{ m_s1.Mesh(Eigen::ArrayXXd::Ones(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };

        b =   F0(u).transpose() * oo * F0(w)
            + F0(u).transpose() * oi * F1(w)
            + F1(u).transpose() * io * F0(w)
            + F1(u).transpose() * ii * F1(w);

        // Assembly
        m_mesh = Lc + Ld.reshaped<Eigen::RowMajor>(MESH_SIZE*3, MESH_SIZE) + b;

        return m_mesh;
    }; 
    std::tuple<Eigen::MatrixXd, Eigen::VectorXi> Patch::GetFemMesh()
    {
        int size{};
        Eigen::MatrixXd nodes{ m_mesh };
        size = (nodes.cols()-1)*4*(nodes.rows()/3-1);  
        Eigen::VectorXi elts{ size };


        for (int i = 0; i < (nodes.rows()/3)-1; i++)
        {
            for (int j = 0; j < nodes.cols()-1; j++)
            {
                int n{};
                int m{};
                n =  i*nodes.cols() + j;
                m = i*(nodes.cols()-1) + j;
                elts(m*4) = n;
                elts(m*4+1) = n+1;
                elts(m*4+2) = n+1+nodes.cols();
                elts(m*4+3) = n+nodes.cols();
            }
        }

        return { m_mesh, elts };

    };
}
