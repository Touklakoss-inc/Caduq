#ifndef CQ_POINT_H
#define CQ_POINT_H

#include "Geometry/Point.h"
#include "XPBD/Point.h"
#include "Entity.h"

#include <memory>
#include <string>
#include <Eigen/Dense>

namespace Caduq
{
    class Point: public Entity
    {
    private:

        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        std::shared_ptr<Geometry::Point> m_GeoPoint;
        std::shared_ptr<XPBD::Point> m_PhyXPoint;

        static inline float m_GuiPointPopupCoord[3] { 0.0f, 0.0f, 0.0f };

    public:
        struct OptParam
        {
            std::string name = "";
            double mass = 1.0;
            bool grounded = false;
        };

        Point(Eigen::Vector3d pos, Type type, OptParam oParam = { .name="", .mass=1.0, .grounded=false });

        ~Point() override = default;

        void Init() override;
        void UpdateGFX() override;
        void Update(double x, double y, double z);
        void Delete() override;

        void RenderImGui(EntityManager& entityManager) override;
        static void Popup(EntityManager& entityManager);
        static void SetPopupParam(Eigen::Vector3f coord) {
            for (int i = 0; i < 3; i++) m_GuiPointPopupCoord[i] = coord[i]; };

        const std::shared_ptr<Geometry::Point>& GetGeoPoint() const { return m_GeoPoint; };
        const std::shared_ptr<XPBD::Point>& GetPhyXPoint() const { return m_PhyXPoint; };
        int GetID() const override { return m_Id; };
    };
}
#endif
