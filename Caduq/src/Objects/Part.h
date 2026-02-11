#ifndef CQ_PART_H
#define CQ_PART_H

#include "Eigen/Geometry"
#include "EntityManager.h"
#include "Frame.h"

#include "Objects/Entity.h"
namespace Caduq 
{
    class Part : public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Frame m_MainFrame;
        EntityManager m_EntityManager{ };

    public:
        Part(Transform transform = Transform::Identity(), Type type = Type::part, const std::string& name = "");
        ~Part() = default;

        void Init() override;
        void UpdateGFX() override;
        void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform) override;
        void Delete() override;
        int GetID() const override { return m_Id; };

        void RenderImGui(EntityManager& _) override;
    };
}

#endif
