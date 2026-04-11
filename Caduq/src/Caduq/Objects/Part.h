#ifndef CQ_PART_H
#define CQ_PART_H

#include "Eigen/Core"
#include "EntityManager.h"
#include "Frame.h"

#include "Caduq/Objects/Entity.h"
#include "XPBD/PhyXPart.h"

#include <Eigen/Dense>

namespace Caduq 
{
    class Part : public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        std::shared_ptr<Frame> m_MainFrame;
        std::shared_ptr<XPBD::PhyXPart> m_PhyXPart;
        EntityManager m_EntityManager;

        /* ImGui */
        static inline float m_GuiPopupPos[3] { 0.0f, 0.0f, 0.0f };
        static inline float m_GuiPopupRot[4] { 0.0f, 0.0f, 0.0f, 0.0f};
        static inline float m_GuiPopupSca[3] { 1.0f, 1.0f, 1.0f };

    public:
        struct OptParam
        {
            Type type = Type::part;
            std::string name = "";
            double mass = 1.0;
            Eigen::Vector3d inertiaTensor = Eigen::Vector3d::Zero();
            bool grounded = false;
        };

        Part(Geometry::Transform transform, const std::shared_ptr<Frame>& frame,  OptParam oParam = { .type = Type::part, .name="", .mass=1.0, .inertiaTensor = Eigen::Vector3d::Zero(), .grounded=false});
        ~Part() = default;

        void Init() override;
        void Update(Eigen::Vector3d position, Eigen::Quaterniond rotation);
        void UpdateGFX() override;
        void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform) override;
        void Delete() override;
        int GetID() const override { return m_Id; };

        void RenderImGui(EntityManager& _) override;
        static void Popup(EntityManager& entityManager);
        static void SetPopupParam(Eigen::Vector3d position, Eigen::Vector4d rotation);

        const std::shared_ptr<Frame>& GetMainFrame() { return m_MainFrame; };
        const std::shared_ptr<XPBD::PhyXPart>& GetPhyXPart() const { return m_PhyXPart; };
        template<typename T> 
        void CreateEntity(const std::shared_ptr<T> &entity) { m_EntityManager.CreateEntity(entity); };
    };
}

#endif
