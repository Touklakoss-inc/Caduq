#ifndef CQ_PATCH_H
#define CQ_PATCH_H

#include <Vizir.h>

#include <memory>
#include <tuple>
#include "Spline.h"
#include "Geometry/Patch.h"

namespace Caduq
{
    class Patch: public Entity
    {
        private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        std::shared_ptr<Spline> m_s0;
        std::shared_ptr<Spline> m_s1;
        std::shared_ptr<Spline> m_s2;
        std::shared_ptr<Spline> m_s3;

        Geometry::Patch m_c0;

        int m_mesh_size{ 10 };

        public:
        Patch(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
              const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3,
              int mesh_size, Type type, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Update(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
                    const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3);

        std::shared_ptr<Spline> GetSpline0() { return m_s0; };
        std::shared_ptr<Spline> GetSpline1() { return m_s1; };
        std::shared_ptr<Spline> GetSpline2() { return m_s2; };
        std::shared_ptr<Spline> GetSpline3() { return m_s3; };

        int GetID() const override { return m_Id; };
    };
}
#endif
