#ifndef PATCH_H
#define PATCH_H

#include <Eigen/Dense>
#include "Spline.h"

namespace Geometry
{
    class Patch
    {
        private:
        Geometry::Spline m_s0; // not good ?
        Geometry::Spline m_s1;
        Geometry::Spline m_s2;
        Geometry::Spline m_s3;

        Eigen::MatrixXd m_mesh{};

        public:
        Patch(Geometry::Spline s0, Geometry::Spline s1, Geometry::Spline s2, Geometry::Spline s3);
        Eigen::VectorXd F1(Eigen::VectorXd t); 
        Eigen::VectorXd F0(Eigen::VectorXd t); 

        Eigen::MatrixXd Mesh(Eigen::VectorXd u, Eigen::VectorXd w, const int MESH_SIZE); 
        std::tuple<Eigen::MatrixXd, Eigen::VectorXi> GetFemMesh();
    };
}

#endif
