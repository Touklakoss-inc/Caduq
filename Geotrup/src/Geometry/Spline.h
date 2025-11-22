#ifndef GO_SPLINE_H
#define GO_SPLINE_H

#include <Eigen/Dense>
#include <tuple>

#include "Geo.h"

namespace Geometry
{
    class Spline
    {
        private:
        SplinePoint m_startPoint{};
        SplinePoint m_endPoint{};

        Eigen::MatrixXd m_equationConsts{ 4, 3 };
        Eigen::MatrixXd m_mesh{ 3, 10 };

        public:
        Spline(SplinePoint startPoint, SplinePoint endPoint);
        Eigen::MatrixXd Mesh(Eigen::ArrayXd u, const int MESH_SIZE);
        std::tuple<Eigen::MatrixXd, Eigen::VectorXi> GetFemMesh();
        Geometry::Mesh GetGfxMesh();
        double GetLength();
    };
}

#endif
