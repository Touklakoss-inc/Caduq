#ifndef XPBD_POINT_H
#define XPBD_POINT_H

#include "Eigen/Core"
#include <Eigen/Dense>

namespace XPBD
{
    class Point
    {
    private:
        Eigen::Vector3d m_Velocity { 0.0, 0.0, 0.0 };
        double m_Mass;
        bool m_Grounded;
        Eigen::Vector3d m_LastPosition { };

    public:
        Point() = default;
        Point(double mass, bool grounded, Eigen::Vector3d velocity = { 0.0, 0.0, 0.0 });

        Eigen::Vector3d GetVelocity() { return m_Velocity; };
        void SetVelocity(Eigen::Vector3d velocity) { m_Velocity = velocity; };

        double GetMass() { return m_Mass; };
        double& GetMassRef() { return m_Mass; };
        void SetMass(double mass) { m_Mass = mass; };

        Eigen::Vector3d GetLastPosition() { return m_LastPosition; };
        void SetLastPosition(Eigen::Vector3d lastPosition) { m_LastPosition = lastPosition; };

        bool IsGrounded() { return m_Grounded; };
        bool& IsGroundedRef() { return m_Grounded; };
    };
}

#endif
