#ifndef CQ_PATCH_H
#define CQ_PATCH_H

#include <Vizir.h>

#include "Spline.h"

namespace Caduq
{
    class Patch: public Entity
    {
        private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Caduq::Spline m_s0;
        Caduq::Spline m_s1;
        Caduq::Spline m_s2;
        Caduq::Spline m_s3;

        int m_mesh_size{ 10 };

        Vizir::Ref<Vizir::VertexArray> m_PatchVertexArray;

        public:
        Patch(const Caduq::Spline& s0, const Caduq::Spline& s1, const Caduq::Spline& s2, const Caduq::Spline& s3,
              int mesh_size, const std::string& name = "");

        void Init();
        void Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform);
    };
}
#endif
