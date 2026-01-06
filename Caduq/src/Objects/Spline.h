#ifndef CQ_SPLINE_H
#define CQ_SPLINE_H

#include <Vizir.h>
#include <Eigen/Core>

#include <memory>
#include "Entity.h"
#include "Point.h"
#include "Geometry/Spline.h"

namespace Caduq
{
    struct PointTangency
    {
        Eigen::RowVector3d tangent{ 0.0, 0.0, 0.0 };
        double tension{ 1.0 };
    };

    class Spline: public Entity
    {
        private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };
        int m_mesh_size{ 10 };

        std::shared_ptr<Point> m_StartPoint;
        PointTangency m_StartTangency;
        std::shared_ptr<Point> m_EndPoint;
        PointTangency m_EndTangency;

        Geometry::Spline m_Spline;

        public:
        Spline(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
               const std::shared_ptr<Point>& endPoint, PointTangency endTangency,
               int mesh_size, Type type, const std::string& name = "");
        ~Spline();

        void Init() override;

        Geometry::Spline GetGeoSpline() const;
    };
}
#endif
