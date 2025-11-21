#ifndef CQPOINT_H
#define CQPOINT_H

#include <Vizir.h>

#include "Eigen/Core"

namespace Caduq
{
    class Point
    {
        private:
        Eigen::Vector3d m_Position{};

        Vizir::Ref<Vizir::VertexArray> m_PointVertexArray;

        glm::mat4 m_Transform;

        public:
        Point(double x, double y, double z);
        Point(Eigen::Vector3d pos);
        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader);
    };
}
#endif
