#ifndef CQ_POINT_H
#define CQ_POINT_H

#include <string>
#include <Vizir.h>

#include "Eigen/Core"

#include "Geometry/Point.h"
#include "Entity.h"
#include "Geometry/Point.h"

namespace Caduq
{
    class Point: public Entity
    {
    private:

        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Geometry::Point m_Point{};

    public:
        Point(double x, double y, double z, Type type, const std::string& name = "");
        Point(Eigen::Vector3d pos, Type type, const std::string& name = "");

        void Init() override;

        Geometry::Point GetGeoPoint() const;
    };
}
#endif
