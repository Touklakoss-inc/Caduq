#ifndef GO_POINT_H
#define GO_POINT_H

#include <Eigen/Dense>

namespace Geometry
{
    class Point
    {
        private:
        double m_x{ 0.0 };
        double m_y{ 0.0 };
        double m_z{ 0.0 };

        public:
        Point() = default;
        Point(double x, double y, double z = 0.0);
        Point(Eigen::Vector3d pos);

        void SetPosition(double x, double y, double z = 0.0);
        Eigen::Vector3d GetPosition() const;

        void Print() const;
    };
}
#endif
