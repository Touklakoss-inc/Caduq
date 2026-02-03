#include "Joint.h"

namespace XPBD
{
    void Joint::ApplyLinearCorrection(Point& p1, Point& p2, Eigen::Vector3d dp, double alpha, double dts)
    {
        double C = dp.norm();
        Eigen::Vector3d n = dp.normalized();

        double w1 = 1.0/p1.GetMass();
        if (p1.IsGrounded())
            w1 = 0.0;

        double w2 = 1.0/p2.GetMass();
        if (p2.IsGrounded())
            w2 = 0.0;

        double lambda = -C;
        if (dts != 0.0 && (w1 != 0.0 || w2 != 0.0 || alpha != 0.0))
            lambda /= w1 + w2 + alpha/(dts*dts);

        p1.GetGeoPoint()->SetPosition(p1.GetGeoPoint()->GetPosition() + lambda*n*w1);
        p2.GetGeoPoint()->SetPosition(p2.GetGeoPoint()->GetPosition() - lambda*n*w2);
    }

    void Joint::Attach(Point& p1, Point& p2, double dRest, double alpha, double dts)
    {
        Eigen::Vector3d n = p2.GetGeoPoint()->GetPosition() - p1.GetGeoPoint()->GetPosition(); 
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(p1, p2, -(d-dRest)*n, alpha, dts);
    }
}
