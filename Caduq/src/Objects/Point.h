#ifndef CQ_POINT_H
#define CQ_POINT_H

#include <string>
#include "Vizir.h"

#include <Eigen/Core>

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

    public:
        Point(double x, double y, double z, Type type, const std::string& name = "");
        Point(Eigen::Vector3d pos, Type type, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Update(double x, double y, double z);

        Geometry::Point GetGeoPoint() const { return m_Point; };
        int GetID() const override { return m_Id; };
    };
}
#endif
