#ifndef JOINT_H
#define JOINT_H

#include "Point.h"

namespace XPBD
{
    class Joint : public std::enable_shared_from_this<Joint>
    {
    protected:
        static inline int s_IdGenerator{ 0 };
        int m_Id;
        std::string m_Name;

        void ApplyLinearCorrection(Point& p1, Point& p2, Eigen::Vector3d dp, double alpha, double dts);

        void Attach(Point& p1, Point& p2, double dRest, double alpha, double dts);

    public:
        Joint();
        virtual ~Joint() { std::cout << "Joint deleted" << '\n'; };

        virtual void Init() = 0;
        virtual void Delete() = 0;
        virtual void ApplyConstraints(double dts) = 0;
        virtual void RenderImGui() = 0;
    };
}

#endif
