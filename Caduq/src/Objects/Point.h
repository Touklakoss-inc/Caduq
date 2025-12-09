#ifndef CQ_POINT_H
#define CQ_POINT_H

#include <Vizir.h>

#include "Eigen/Core"

#include "Geometry/Point.h"

namespace Caduq
{
    class Point
    {
        private:
        Geometry::Point m_Point{};

        Vizir::Ref<Vizir::VertexArray> m_PointVertexArray;

        public:
        Point(double x, double y, double z);
        Point(Eigen::Vector3d pos);

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
        Geometry::Point GetGeoPoint() const;
    };
}
#endif
