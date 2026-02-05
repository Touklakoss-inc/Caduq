#ifndef XPBD_POINT_H
#define XPBD_POINT_H

#include "Geometry/Point.h"

#include <memory>
#include <iostream>

namespace XPBD
{
    class Joint;
    class Point
    {
    private:
        std::shared_ptr<Geometry::Point> m_GeoPoint;
        Eigen::Vector3d m_Velocity { 0.0, 0.0, 0.0 };
        double m_Mass;
        bool m_Grounded;
        Eigen::Vector3d m_LastPosition { };

        std::string m_Name;
        int m_Id;

        std::vector<std::shared_ptr<Joint>> m_ParentJoints { };

    public:
        Point() = default;
        Point(const std::shared_ptr<Geometry::Point>& geoPoint, double mass, const std::string& name, int id, bool grounded = false, Eigen::Vector3d velocity = { 0.0, 0.0, 0.0 });
        ~Point() { std::cout << "PhyXPoint deleted" << '\n'; };

        void Delete();

        const std::shared_ptr<Geometry::Point>& GetGeoPoint() { return m_GeoPoint; };

        Eigen::Vector3d GetVelocity() { return m_Velocity; };
        void SetVelocity(Eigen::Vector3d velocity) { m_Velocity = velocity; };

        double GetMass() { return m_Mass; };
        double& GetMassRef() { return m_Mass; };
        void SetMass(double mass) { m_Mass = mass; };

        Eigen::Vector3d GetLastPosition() { return m_LastPosition; };
        void SetLastPosition(Eigen::Vector3d lastPosition) { m_LastPosition = lastPosition; };

        bool IsGrounded() { return m_Grounded; };
        bool& IsGroundedRef() { return m_Grounded; };

        void AddParentJoint(const std::shared_ptr<Joint>& joint) { m_ParentJoints.push_back(joint); };
        void RemoveParentJoint(const std::shared_ptr<Joint>& joint);

        const auto& GetParentJoints() { return m_ParentJoints; };

        const std::string& GetName() { return m_Name; };
        int GetID() { return m_Id; };
    };
}

#endif
