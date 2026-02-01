#include "Point.h"

namespace XPBD
{
    Point::Point(double mass, Eigen::Vector3d velocity)
        : m_Mass{ mass }, m_Velocity{ velocity }
    {
    }
}
