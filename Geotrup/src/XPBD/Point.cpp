#include "Point.h"

namespace XPBD
{
    Point::Point(double mass, bool grounded, Eigen::Vector3d velocity)
        : m_Mass{ mass }, m_Grounded{ grounded }, m_Velocity{ velocity }
    {
    }
}
