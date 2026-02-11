#ifndef CQ_PATCH_H
#define CQ_PATCH_H

#include "Entity.h"
#include "EntityManager.h"
#include "Spline.h"
#include "Geometry/Patch.h"

#include <memory>

namespace Caduq
{
    class Patch: public Entity
    {
        private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        std::shared_ptr<Spline> m_s0;
        std::shared_ptr<Spline> m_s1;
        std::shared_ptr<Spline> m_s2;
        std::shared_ptr<Spline> m_s3;

        Geometry::Patch m_c0{ };

        int m_mesh_size{ 10 };

        /* ImGui */
        static inline int m_GuiSpline1ID{ 0 }; // Here we store our selection data as an index.
        static inline int m_GuiSpline2ID{ 0 }; // Here we store our selection data as an index.
        static inline int m_GuiSpline3ID{ 0 }; // Here we store our selection data as an index.
        static inline int m_GuiSpline4ID{ 0 }; // Here we store our selection data as an index.

        public:
        Patch(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
              const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3,
              int mesh_size, Type type = Type::patch, const std::string& name = "");
        ~Patch() override = default;

        void Init() override;
        void UpdateGFX() override;
        void Update(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
                    const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3);

        void RenderImGui(EntityManager& entityManager) override;
        static void PatchPopup(EntityManager& entityManager);
        static void SetPopupParam(EntityManager& entityManager, int spline1ID, int spline2ID, int spline3ID, int spline4ID);

        std::shared_ptr<Spline> GetSpline0() { return m_s0; };
        std::shared_ptr<Spline> GetSpline1() { return m_s1; };
        std::shared_ptr<Spline> GetSpline2() { return m_s2; };
        std::shared_ptr<Spline> GetSpline3() { return m_s3; };

        int GetID() const override { return m_Id; };
    };
}
#endif
