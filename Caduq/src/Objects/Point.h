#ifndef CQ_POINT_H
#define CQ_POINT_H

#include <string>
#include <Vizir.h>

#include "Eigen/Core"

#include "Geometry/Point.h"
#include "Entity.h"

namespace Caduq
{
    class Point: public Entity
    {
    private:

        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Geometry::Point m_Point{};

        Vizir::Ref<Vizir::VertexArray> m_PointVertexArray;

    public:
        Point(double x, double y, double z, const std::string& name = "");
        Point(Eigen::Vector3d pos, const std::string& name = "");

        void Init() override;
        void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform) override;

        Geometry::Point GetGeoPoint() const;
    };
}
#endif
