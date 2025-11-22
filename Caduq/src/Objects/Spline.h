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
        Geometry::Spline m_spline;

        int m_mesh_size{ 10 };

        Vizir::Ref<Vizir::VertexArray> m_SplineVertexArray;

        public:
        Spline(Caduq::Point startPoint, PointTangency startTangency, 
               Caduq::Point endPoint, PointTangency endTangency,
               int mesh_size);

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
        Geometry::Spline GetGeoSpline();
    };
}
#endif
