#ifndef CQ_PART_H
#define CQ_PART_H

#include "EntityManager.h"
#include "Frame.h"

#include "Caduq/Objects/Entity.h"

namespace Caduq 
{
    class Part : public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        std::shared_ptr<Frame> m_MainFrame;
        EntityManager m_EntityManager;

        /* ImGui */
        static inline float m_GuiPopupPos[3] { 0.0f, 0.0f, 0.0f };
        static inline float m_GuiPopupRot[4] { 0.0f, 0.0f, 0.0f, 0.0f};
        static inline float m_GuiPopupSca[3] { 1.0f, 1.0f, 1.0f };

    public:
        Part(Geometry::Transform transform, const std::shared_ptr<Frame>& frame, Type type = Type::part, const std::string& name = "");
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
    };
}

#endif
