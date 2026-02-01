#include "PhyXManager.h"

#include "Eigen/Core"
#include "EntityManager.h"
#include <cmath>
namespace Caduq
{
    void PhyXManager::UpdatePhyX(EntityManager& entityManager, float dt, ushort nSubStep)
    {
        double dts = static_cast<double>(dt)/static_cast<double>(nSubStep);
        for (ushort n = 0; n < nSubStep; n++)
        {
            for (const auto& point : entityManager.GetPointList())
            {
                auto& phyXPoint = point->GetPhyXPointRef(); 
                if (!phyXPoint.IsGrounded())
                {
                    auto& geoPoint = point->GetGeoPointRef();
                    phyXPoint.SetVelocity(phyXPoint.GetVelocity() + dts*g);

                    phyXPoint.SetLastPosition(geoPoint.GetPosition());

                    geoPoint.SetPosition(geoPoint.GetPosition() + dts*phyXPoint.GetVelocity());
                }
            }

            Attach(*entityManager.GetPoint(0), *entityManager.GetPoint(1), std::sqrt(2.0), 0.0, dts);
            Attach(*entityManager.GetPoint(1), *entityManager.GetPoint(2), std::sqrt(1.5), 0.0, dts);
            Attach(*entityManager.GetPoint(2), *entityManager.GetPoint(3), std::sqrt(2.0), 0.0, dts);

            for (const auto& point : entityManager.GetPointList())
            {
                auto& phyXPoint = point->GetPhyXPointRef(); 
                if (!phyXPoint.IsGrounded())
                {
                    auto& geoPoint = point->GetGeoPointRef();
                    phyXPoint.SetVelocity((geoPoint.GetPosition() - phyXPoint.GetLastPosition())/dts);
                }
            }
        }
    }

    void PhyXManager::ApplyLinearCorrection(Caduq::Point& p1, Caduq::Point& p2, Eigen::Vector3d dp, double alpha, double dts)
    {
        double C = dp.norm();
        Eigen::Vector3d n = dp.normalized();

        double w1 = 1/p1.GetPhyXPointRef().GetMass();
        double w2 = 1/p2.GetPhyXPointRef().GetMass();

        double lambda = -C / (w1 + w2 + alpha/(dts*dts));

        p1.GetGeoPointRef().SetPosition(p1.GetGeoPoint().GetPosition() + lambda*n*w1);
        p2.GetGeoPointRef().SetPosition(p2.GetGeoPoint().GetPosition() - lambda*n*w2);
    }

    void PhyXManager::Attach(Caduq::Point& p1, Caduq::Point& p2, double dRest, double alpha, double dts)
    {
        Eigen::Vector3d n = p2.GetGeoPoint().GetPosition() - p1.GetGeoPoint().GetPosition(); 
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(p1, p2, -(d-dRest)*n, alpha, dts);
    }
}
