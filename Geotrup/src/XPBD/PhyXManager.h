#ifndef PHYXMANAGER_H
#define PHYXMANAGER_H

#include "Eigen/Core"
#include "PhyXPart.h"
#include "JointsBuildingBlocks.h"

#include <Eigen/Dense>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace XPBD
{

    class PhyXManager
    {
    private:
        const Eigen::Vector3d g { 0.0, -9.81, 0.0 };

        bool m_TimeEnabled { false };

        std::vector<std::shared_ptr<PhyXPart>> m_PhyXPartList { };

        // ImGui function
        void AttachPopup();

        // Attach popup parameters
        int m_GuiStartPointID { 0 };
        int m_GuiEndPointID { 0 };
        double m_GuiDistRest[1] { 0.0 };

        int m_GuiSubSteps { 500 };

        int m_PartCount { 0 };

        // current entities translation positions
        std::vector<double> m_PtXPosition {};
        std::vector<double> m_PtYPosition {};
        std::vector<double> m_PtZPosition {};
        // last entities translation positions
        std::vector<double> m_PtXLastPos {};
        std::vector<double> m_PtYLastPos {};
        std::vector<double> m_PtZLastPos {};

        // current entities rotation positions
        std::vector<double> m_PtXRotation {};
        std::vector<double> m_PtYRotation {};
        std::vector<double> m_PtZRotation {};
        std::vector<double> m_PtWRotation {};
        // last entities rotation positions
        std::vector<double> m_PtXLastRot {};
        std::vector<double> m_PtYLastRot {};
        std::vector<double> m_PtZLastRot {};
        std::vector<double> m_PtWLastRot {};

        // current entities linear velcity
        std::vector<double> m_PtXLinVelocity {};
        std::vector<double> m_PtYLinVelocity {};
        std::vector<double> m_PtZLinVelocity {};
        // current entities angular velcity
        std::vector<double> m_PtXAngVelocity {};
        std::vector<double> m_PtYAngVelocity {};
        std::vector<double> m_PtZAngVelocity {};

        // entities masses
        std::vector<double> m_PtInvMasses {};

        // entities inertia tensor
        std::vector<double> m_PtXInvInertia {};
        std::vector<double> m_PtYInvInertia {};
        std::vector<double> m_PtZInvInertia {};

        // is grounded mask
        std::vector<uint8_t> m_PtGrounded {};

        // joints
        std::vector<JAttach> m_JsAttach {};
        std::vector<JRestrictAxis> m_JsRestrictAxis {};

        void ImportEntities();

    public:
        static inline bool s_PhyXEnabled { false };
        typedef std::variant<JAttach, JRestrictAxis> Joint;

        PhyXManager() = default;

        void UpdatePhyX(float dt);

        void CreateJoint(const std::shared_ptr<PhyXPart> p1, const std::shared_ptr<PhyXPart> p2, Joint joint);

        void AddPhyXPartToList(const std::shared_ptr<PhyXPart>& phyXPart) { m_PhyXPartList.push_back(phyXPart); };

        void RenderImGui();

    private:
        void Integrate(int p, double dt);
        double GetInverseMass(int p, Eigen::Vector3d normal, Eigen::Vector3d pos);
        void _ApplyLinearCorrection(int p, Eigen::Vector3d corr, Eigen::Vector3d pos);
        double ApplyLinearCorrection(int p1, Eigen::Vector3d r1, std::optional<int> p2, Eigen::Vector3d r2, Eigen::Vector3d corr, double compliance, double dt);
        void SolveConstraint(double dt, int p1, std::optional<int> p2, Eigen::Vector3d r1, Eigen::Vector3d r2, double dRest, double alpha);
        void UpdateVelocities(int p, double dt, double damping);

        void Attach(const JAttach& j, double dt);
        void RestrictToAxis(const JRestrictAxis& j, double dt);
    };
}

#endif
