#ifndef CQ_SPLINE_H
#define CQ_SPLINE_H

#include <Vizir.h>
#include <Eigen/Core>

#include "Point.h"
#include "Geometry/Spline.h"

namespace Caduq
{
    struct PointTangency
    {
        Eigen::RowVector3d tangent{ 0.0, 0.0, 0.0 };
        double tension{ 0.0 };
    };

    class Spline: public Entity
    {
        private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };
        int m_mesh_size{ 10 };

        Geometry::Spline m_spline;

        Vizir::Ref<Vizir::VertexArray> m_SplineVertexArray;

        public:
        Spline(const Caduq::Point& startPoint, PointTangency startTangency, 
               const Caduq::Point& endPoint, PointTangency endTangency,
               int mesh_size, const std::string& name = "");

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
        Geometry::Spline GetGeoSpline() const;
    };
}
#endif
