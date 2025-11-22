#ifndef CQ_PATCH_H
#define CQ_PATCH_H

#include <Vizir.h>

#include "Eigen/Core"
#include "Spline.h"

namespace Caduq
{
    class Patch
    {
        private:
        Caduq::Spline m_s0;
        Caduq::Spline m_s1;
        Caduq::Spline m_s2;
        Caduq::Spline m_s3;

        int m_mesh_size{ 10 };

        Vizir::Ref<Vizir::VertexArray> m_PatchVertexArray;

        public:
        Patch(Caduq::Spline s0, Caduq::Spline s1, Caduq::Spline s2, Caduq::Spline s3, int mesh_size);

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
    };
}
#endif
