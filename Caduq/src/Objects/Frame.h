#ifndef CQ_FRAME_H
#define CQ_FRAME_H

#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Entity.h"

#include <Eigen/Dense>

namespace Caduq
{
    typedef Eigen::Transform<double, 3, Eigen::TransformTraits::Affine> Transform;
    class Frame: public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Transform m_Transform;

        Eigen::Vector3d m_X;
        Eigen::Vector3d m_Y;
        Eigen::Vector3d m_Z;

        void UpdateFrame();

    public:
        Frame(Transform transform, Type type, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Delete() override;

        void Translate(Eigen::Vector3d translation);
        void TranslateLocal(Eigen::Vector3d translation);
        void Rotate(Eigen::Quaterniond rotation);
        void RotateLocal(Eigen::Quaterniond rotation);

        void RenderImGui(EntityManager& entityManager) override;

        int GetID() const override { return m_Id; };
        Eigen::Vector3d GetZ() { return m_Z; };
    };
}
#endif
