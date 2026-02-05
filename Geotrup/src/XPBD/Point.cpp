#include "Point.h"

#include "Joint.h"

namespace XPBD
{
    Point::Point(const std::shared_ptr<Geometry::Point>& geoPoint, double mass, const std::string& name, int id, bool grounded, Eigen::Vector3d velocity)
        : m_GeoPoint{ geoPoint }, m_Mass{ mass }, m_Name{ name }, m_Id{ id }, m_Grounded{ grounded }, m_Velocity{ velocity }
    {
    }

    void Point::Delete()
    {
        m_ParentJoints.clear();
        m_GeoPoint = nullptr;
    }

    void Point::RemoveParentJoint(const std::shared_ptr<Joint>& joint)
    {
        auto it = std::find(m_ParentJoints.begin(), m_ParentJoints.end(), joint);
        if (it != m_ParentJoints.end())
        {
            m_ParentJoints.erase(it);
        }
    }
}
