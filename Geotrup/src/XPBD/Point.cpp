#include "Point.h"

namespace XPBD
{
    Point::Point(const std::shared_ptr<Geometry::Point>& geoPoint, double mass, bool grounded, Eigen::Vector3d velocity)
        : m_GeoPoint{ geoPoint }, m_Mass{ mass }, m_Grounded{ grounded }, m_Velocity{ velocity }
    {
    }
}
