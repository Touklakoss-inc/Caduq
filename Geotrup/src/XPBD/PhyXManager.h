#ifndef PHYXMANAGER_H
#define PHYXMANAGER_H

#include "PhyXPart.h"
#include <Eigen/Dense>

#include <memory>
#include <sys/types.h>
#include <vector>
namespace XPBD
{
    struct JAttachStruct
    {
        int pt1;
        int pt2;
        double m_DRest;
        double m_Alpha;
    };

    class PhyXManager
    {
    private:
        const Eigen::Vector3d g { 0.0, 9.81, 0.0 };

        bool m_TimeEnabled { false };

        std::vector<std::shared_ptr<PhyXPart>> m_PhyXPartList { };

        // ImGui function
        void AttachPopup();

        // Attach popup parameters
        int m_GuiStartPointID { 0 };
        int m_GuiEndPointID { 0 };
        double m_GuiDistRest[1] { 0.0 };

        int m_GuiSubSteps { 500 };


        // current entities positions
        std::vector<double> m_PtXPosition {};
        std::vector<double> m_PtYPosition {};
        std::vector<double> m_PtZPosition {};

        // last entities positions
        std::vector<double> m_PtXLastPos {};
        std::vector<double> m_PtYLastPos {};
        std::vector<double> m_PtZLastPos {};

        // current entities velcity
        std::vector<double> m_PtXVelocity {};
        std::vector<double> m_PtYVelocity {};
        std::vector<double> m_PtZVelocity {};

        // is grounded mask
        std::vector<uint8_t> m_PtGrounded {};

        // entities masses
        std::vector<double> m_PtMasses {};

        // joints
        std::vector<JAttachStruct> m_Joints {};

        void ImportEntities();

        void AttachJoint(int p1, int p2, double dRest, double alpha, double dts);

        void ApplyLinearCorrection(int p1, int p2, Eigen::Vector3d dp, double alpha, double dts);

    public:
        static inline bool s_PhyXEnabled { false };

        PhyXManager() = default;

        void UpdatePhyX(float dt);

        void CreateJoint(const std::shared_ptr<PhyXPart> p1, const std::shared_ptr<PhyXPart> p2, double dRest, double alpha);

        void AddPhyXPartToList(const auto& phyXPart) { m_PhyXPartList.push_back(phyXPart); };

        void RenderImGui();
    };
}

#endif
