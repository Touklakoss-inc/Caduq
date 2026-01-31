#include "Patch.h"
#include <Eigen/Core>
#include <iostream>

namespace Geometry
{
    Patch::Patch(const Spline& s0, const Spline& s1, const Spline& s2, const Spline& s3)
        : m_s0{ s0 }, m_s1{ s1 }, m_s2{ s2 }, m_s3{ s3 }
    {
    }

    Patch::Patch(const Spline& s0, const Spline& s1, const Spline& s2)
        : m_s0{ s0 }, m_s1{ s1 }, m_s2{ s2 }, m_s3{ s0.GetEndPoint(), s0.GetEndPoint() }
    {
    }

    // Interpolation functions
    // Curvature continuity between multiple patches
    Eigen::VectorXd Patch::F1(Eigen::VectorXd t)
    {
        return (10*t.array().pow(3) - 15*t.array().pow(4) + 6*t.array().pow(5)).matrix();
    }; 

    double Patch::F1(double t)
    {
        return 10*t*t*t - 15*t*t*t*t + 6*t*t*t*t*t;
    };

    Eigen::VectorXd Patch::F0(Eigen::VectorXd t)
    {
        return (1 - F1(t).array()).matrix();
    };

    double Patch::F0(double t)
    {
        return 1 - F1(t);
    };

    Eigen::MatrixXd Patch::Mesh(Eigen::VectorXd u, Eigen::VectorXd w, const int MESH_SIZE)
    {
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
        Eigen::MatrixXd cc{ MESH_SIZE*3, MESH_SIZE };
        Eigen::MatrixXd oo{ m_s0.Mesh(Eigen::ArrayXd::Zero(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd oi{ m_s0.Mesh(Eigen::ArrayXd::Ones(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd io{ m_s1.Mesh(Eigen::ArrayXd::Zero(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };
        Eigen::MatrixXd ii{ m_s1.Mesh(Eigen::ArrayXd::Ones(MESH_SIZE), MESH_SIZE).reshaped(MESH_SIZE*3, 1) };

        Eigen::MatrixXd F0w{ 3, MESH_SIZE };
        F0w.row(0) = F0(w);
        F0w.row(1) = F0(w);
        F0w.row(2) = F0(w);

        Eigen::MatrixXd F1w{ 3, MESH_SIZE };
        F1w.row(0) = F1(w);
        F1w.row(1) = F1(w);
        F1w.row(2) = F1(w);

        // cc = (F0w.reshaped(MESH_SIZE*3, 1).array() * oo.array()).matrix() * F0(u).transpose()
        //   + (F1w.reshaped(MESH_SIZE*3, 1).array() * oi.array()).matrix() * F0(u).transpose()
        //   + (F0w.reshaped(MESH_SIZE*3, 1).array() * io.array()).matrix() * F1(u).transpose()
        //   + (F1w.reshaped(MESH_SIZE*3, 1).array() * ii.array()).matrix() * F1(u).transpose();

        for (int r = 0; r < MESH_SIZE*3; r+=3) 
        {
            for (int c = 0; c < MESH_SIZE; c++)
            {
                b(r, c) = F0(u(c)) * oo(0) * F0(w(r/3))
                        + F0(u(c)) * oi(0) * F1(w(r/3))
                        + F1(u(c)) * io(0) * F0(w(r/3))
                        + F1(u(c)) * ii(0) * F1(w(r/3));
                b(r+1, c) = F0(u(c)) * oo(1) * F0(w(r/3))
                          + F0(u(c)) * oi(1) * F1(w(r/3))
                          + F1(u(c)) * io(1) * F0(w(r/3))
                          + F1(u(c)) * ii(1) * F1(w(r/3));
                b(r+2, c) = F0(u(c)) * oo(2) * F0(w(r/3))
                          + F0(u(c)) * oi(2) * F1(w(r/3))
                          + F1(u(c)) * io(2) * F0(w(r/3))
                          + F1(u(c)) * ii(2) * F1(w(r/3));
            }
        }

        // Assembly
        m_mesh = Lc + Ld.reshaped<Eigen::RowMajor>(MESH_SIZE*3, MESH_SIZE) - b;

        return m_mesh;
    }; 

    std::tuple<Eigen::MatrixXd, Eigen::VectorXi> Patch::GetFemMesh() const
    {
        int size{};
        Eigen::MatrixXd nodes{ m_mesh };
        size = (static_cast<uint32_t>(nodes.cols())-1)*4*(static_cast<uint32_t>(nodes.rows())/3-1);
        Eigen::VectorXi elts{ size };


        // this writes nodes in column dominated order, or in raw array, the data is stored in row dominated order
        // for (int i = 0; i < (nodes.rows()/3)-1; i++)
        // {
        //     for (int j = 0; j < nodes.cols()-1; j++)
        //     {
        //         int n{};
        //         int m{};
        //         n =  i*nodes.cols() + j;
        //         m = i*(nodes.cols()-1) + j;
        //         elts(m*4) = n;
        //         elts(m*4+1) = n+1;
        //         elts(m*4+2) = n+1+nodes.cols();
        //         elts(m*4+3) = n+nodes.cols();
        //         std::cout << n << ", " << nodes(i, j) << '\n';
        //     }
        // }

        // row dominated order
        for (int j = 0; j < nodes.cols()-1; j++)
        {
            for (int i = 0; i < (nodes.rows()/3)-1; i++)
            {
                int n{};
                int m{};
                n =  i* static_cast<int>(nodes.cols()) + j;
                m = i*(static_cast<int>(nodes.cols())-1) + j;
                elts(m*4) = n;
                elts(m*4+1) = n+1;
                elts(m*4+2) = n+1+ static_cast<int>(nodes.rows())/3;
                elts(m*4+3) = n+ static_cast<int>(nodes.rows())/3;
            }
        }

        return { m_mesh, elts };

    };

    struct Mesh Patch::GetGfxMesh() const
    {
        int size{};
        Eigen::MatrixXd nodes{ m_mesh };
        size = (static_cast<int>(nodes.cols())-1)*6*(static_cast<int>(nodes.rows())/3-1);
        Eigen::VectorX<uint32_t> elts{ size };


        for (int j = 0; j < nodes.cols()-1; j++)
        {
            for (int i = 0; i < (nodes.rows()/3)-1; i++)
            {
                int n{};
                int m{};
                n =  i* static_cast<int>(nodes.cols()) + j;
                m = i*(static_cast<int>(nodes.cols())-1) + j;
                elts(m*6) = n;
                elts(m*6+1) = n+1;
                elts(m*6+2) = n+1+ static_cast<uint32_t>(nodes.rows())/3;
                elts(m*6+3) = n;
                elts(m*6+4) = n+1+ static_cast<uint32_t>(nodes.rows())/3;
                elts(m*6+5) = n+ static_cast<uint32_t>(nodes.rows())/3;
            }
        }

        return { m_mesh, elts };
    };
}
