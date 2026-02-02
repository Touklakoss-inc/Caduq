#ifndef PHYXMANAGER_H
#define PHYXMANAGER_H

#include "Eigen/Core"
#include "EntityManager.h"

#include "Point.h"
#include <Eigen/Dense>

namespace Caduq
{
    class PhyXManager
    {
    private:
        const Eigen::Vector3d g { 0.0, 9.81, 0.0 };

        bool m_TimeEnabled { false };

    public:
        static inline bool s_PhyXEnabled { false };

        PhyXManager() = default;

        void RenderImGui();
        void UpdatePhyX(EntityManager& entityManager, float dt, ushort nSubStep);

        void ApplyLinearCorrection(Caduq::Point& p1, Caduq::Point& p2, Eigen::Vector3d dp, double alpha, double dts);
        void Attach(Caduq::Point& p1, Caduq::Point& p2, double dRest, double alpha, double dts);
    };
}

#endif
