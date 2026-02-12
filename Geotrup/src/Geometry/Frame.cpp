#include "Frame.h"

namespace Geometry
{
    Frame::Frame(Transform transform)
        : m_Transform{ transform }
    {
    }

    void Frame::SetPositionRotation(Eigen::Vector3d position, Eigen::Quaterniond rotation)
    {
        m_Transform.setIdentity();
        m_Transform.prerotate(rotation);
        m_Transform.pretranslate(position);
    }
}
