#include <iostream>

#include "Point.h"

namespace Geometry
{
    Point::Point(double x, double y, double z)
        : m_x{ x }, m_y{ y }, m_z{ z }
    {
    }

    void Point::SetPosition(double x, double y, double z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    Eigen::Vector3d Point::GetPosition() 
    { 
        return Eigen::Vector3d{ m_x, m_y, m_z }; 
    }

    void Point::Print() const
    {
        std::cout << "[" << m_x << ", " << m_y << ", " << m_z << "]\n";
    }
}
