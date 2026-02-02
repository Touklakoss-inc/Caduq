#ifndef JOINT_H
#define JOINT_H

#include "Point.h"

namespace XPBD
{
    class Joint
    {
    protected:
        void ApplyLinearCorrection(Point& p1, Point& p2, Eigen::Vector3d dp, double alpha, double dts);

        void Attach(Point& p1, Point& p2, double dRest, double alpha, double dts);

    public:
        Joint() = default;

        virtual void ApplyConstraints(double dts) = 0;
    };
}

#endif
