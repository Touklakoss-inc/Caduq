#ifndef CQ_STLENTITY_H
#define CQ_STLENTITY_H

#include "Entity.h"
#include "Geometry/Geo.h"

namespace Caduq
{
    class StlEntity : public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        static inline std::string m_FileName { "" };
        Geometry::Mesh m_Mesh;

    public:
        StlEntity(const std::string& filename, const std::shared_ptr<Frame>& frame, Type type = Type::stlEntity, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Delete() override;

        void RenderImGui(EntityManager& entityManager) override;
        static void Popup(EntityManager& entityManager);
        static void SetPopupParam(const std::string& name) { m_FileName = name; }; 

        int GetID() const override { return m_Id; };
    };

}

#endif
