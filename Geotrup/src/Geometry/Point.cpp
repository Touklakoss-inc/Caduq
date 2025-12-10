#include <iostream>

#include "Point.h"

#include "Eigen/Core"
namespace Geometry
{
    Point::Point(double x, double y, double z)
        : m_x{ x }, m_y{ y }, m_z{ z }
    {
    }

    Point::Point(Eigen::Vector3d pos)
        : m_x{ pos(0) }, m_y{ pos(1) }, m_z{ pos(2) }
    {
    }

    void Point::SetPosition(double x, double y, double z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    Eigen::Vector3d Point::GetPosition() const 
    { 
        return Eigen::Vector3d{ m_x, m_y, m_z }; 
    }

    void Point::Print() const
    {
        std::cout << "[" << m_x << ", " << m_y << ", " << m_z << "]\n";
    }
}
