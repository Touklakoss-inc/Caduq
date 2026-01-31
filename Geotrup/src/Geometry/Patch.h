#ifndef PATCH_H
#define PATCH_H

#include <Eigen/Dense>

#include "Spline.h"
#include "Geo.h"

namespace Geometry
{
    class Patch
    {
        private:
        Spline m_s0; // not good ?
        Spline m_s1;
        Spline m_s2;
        Spline m_s3;

        Eigen::MatrixXd m_mesh{};

        public:
        Patch() = default;
        Patch(const Spline& s0, const Spline& s1, const Spline& s2, const Spline& s3);

        Eigen::VectorXd F1(Eigen::VectorXd t); 
        double F1(double t); 

        Eigen::VectorXd F0(Eigen::VectorXd t); 
        double F0(double t); 

        Eigen::MatrixXd Mesh(Eigen::VectorXd u, Eigen::VectorXd w, int MESH_SIZE); 
        std::tuple<Eigen::MatrixXd, Eigen::VectorXi> GetFemMesh() const;
        struct Mesh GetGfxMesh() const;
    };
}

#endif
