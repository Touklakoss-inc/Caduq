#ifndef PHYXMANAGER_H
#define PHYXMANAGER_H

#include "Eigen/Core"
#include "EntityManager.h"

#include "XPBD/Joint.h"
#include <Eigen/Dense>

#include <memory>
#include <vector>
namespace Caduq
{
    class PhyXManager
    {
    private:
        const Eigen::Vector3d g { 0.0, 9.81, 0.0 };

        bool m_TimeEnabled { false };

        std::vector<std::shared_ptr<XPBD::Joint>> m_JointList { };
        static inline std::vector<std::shared_ptr<XPBD::Joint>> m_JointsToDelete { };

        // ImGui function
        void AttachPopup(const EntityManager& entityManager);
    public:
        static inline bool s_PhyXEnabled { false };

        PhyXManager() = default;

        void UpdatePhyX(EntityManager& entityManager, float dt, ushort nSubStep);

        void CreateJoint(const std::shared_ptr<XPBD::Joint>& joint);
        static void AddJointToDelete(const std::shared_ptr<XPBD::Joint>& joint) { m_JointsToDelete.push_back(joint); };
        void ClearJointsToDelete();

        void RenderImGui(const EntityManager& entityManager);

        const auto& GetJointList() { return m_JointList; };
    };
}

#endif
