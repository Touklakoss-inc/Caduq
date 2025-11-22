#ifndef CQ_SPLINE_H
#define CQ_SPLINE_H

#include <Vizir.h>

#include "Eigen/Core"
#include "Point.h"

#include "Geometry/Spline.h"
namespace Caduq
{
    struct PointTangency
    {
        Eigen::RowVector3d tangent{ 0.0, 0.0, 0.0 };
        double tension{ 0.0 };
    };

    class Spline
    {
        private:
        Caduq::Point m_start;
        PointTangency m_startTangency{};
        Caduq::Point m_end;
        PointTangency m_endTangency{};

        Vizir::Ref<Vizir::VertexArray> m_SplineVertexArray;

        public:
        Spline(Caduq::Point startPoint, PointTangency startTangency, Caduq::Point endPoint, PointTangency endTangency);

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
    };
}
#endif
