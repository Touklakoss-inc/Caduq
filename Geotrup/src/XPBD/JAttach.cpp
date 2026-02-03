#include "JAttach.h"

#include <memory>
namespace XPBD
{
    JAttach::JAttach(const std::shared_ptr<Point>& p1, const std::shared_ptr<Point>& p2, double dRest, double alpha)
        : m_P1{ p1 }, m_P2{ p2 }, m_DRest{ dRest }, m_Alpha{ alpha }
    {
    }

    void JAttach::Init()
    {
        m_P1->AddParentJoint(shared_from_this());
        m_P2->AddParentJoint(shared_from_this());
    }

    void JAttach::Delete()
    {
        m_P1->RemoveParentJoint(shared_from_this());
        m_P2->RemoveParentJoint(shared_from_this());

        m_P1 = nullptr;
        m_P2 = nullptr;
    }

    void JAttach::ApplyConstraints(double dts)
    {
        Eigen::Vector3d n = m_P2->GetGeoPoint()->GetPosition() - m_P1->GetGeoPoint()->GetPosition(); 
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(*m_P1, *m_P2, -(d-m_DRest)*n, m_Alpha, dts);
    }
}
