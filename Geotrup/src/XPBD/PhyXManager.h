#ifndef PHYXMANAGER_H
#define PHYXMANAGER_H

#include "Point.h"
#include <Eigen/Dense>

#include <memory>
#include <vector>
namespace XPBD
{
    class Joint;
    class PhyXManager
    {
    private:
        const Eigen::Vector3d g { 0.0, 9.81, 0.0 };

        bool m_TimeEnabled { false };

        std::vector<std::shared_ptr<Point>> m_PhyXPointList { };

        std::vector<std::shared_ptr<Joint>> m_JointList { };
        static inline std::vector<std::shared_ptr<Joint>> m_JointsToDelete { };
        std::shared_ptr<Joint> m_CurJoint { nullptr };

        // ImGui function
        void AttachPopup();

        // Attach popup parameters
        int start_point_idx { 0 };
        int end_point_idx { 0 };
        double d_rest[1] { 0.0 };
    public:
        static inline bool s_PhyXEnabled { false };

        PhyXManager() = default;

        void UpdatePhyX(float dt, ushort nSubStep);

        void CreateJoint(const std::shared_ptr<Joint>& joint);
        static void AddJointToDelete(const std::shared_ptr<Joint>& joint) { m_JointsToDelete.push_back(joint); };
        void ClearJointsToDelete();

        void SetCurJoint(const std::shared_ptr<Joint>& joint) { m_CurJoint = joint; };

        const auto& GetJointList() { return m_JointList; };

        void AddPhyXPointToList(const auto& phyXPoint) { m_PhyXPointList.push_back(phyXPoint); };
        void RemovePhyXPointFromList(const std::shared_ptr<Point>& phyXPoint);

        void RenderImGui();
        void SetAttachPopupParam(int start_idx, int end_idx, double dist);
    };
}

#endif
