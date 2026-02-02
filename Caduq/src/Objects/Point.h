#ifndef CQ_POINT_H
#define CQ_POINT_H

#include <string>
#include "Vizir.h"

#include <Eigen/Dense>

#include "Geometry/Point.h"
#include "Vizir/Renderer/VertexArray.h"
#include "XPBD/Point.h"
#include "Entity.h"

namespace Caduq
{
    class Point: public Entity
    {
    private:

        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Geometry::Point m_Point;
        XPBD::Point m_PhyXPoint;

        void UpdateGFXBuffer(Eigen::MatrixXf vertices, Eigen::VectorX<uint32_t> indices, Vizir::PrimitiveType primitiveType=Vizir::POINTS);


    public:
        struct OptParam
        {
            const std::string& name = "";
            double mass = 1.0;
            bool grounded = false;
        };

        Point(Eigen::Vector3d pos, Type type, OptParam oParam = { .name="", .mass=1.0, .grounded=false });

        void Init() override;
        void UpdateGFX() override;
        void Update(double x, double y, double z);

        void RenderImGui(EntityManager& entityManager) override;

        Geometry::Point GetGeoPoint() const { return m_Point; };
        Geometry::Point& GetGeoPointRef() { return m_Point; };
        XPBD::Point& GetPhyXPointRef() { return m_PhyXPoint; };
        int GetID() const override { return m_Id; };
    };
}
#endif
