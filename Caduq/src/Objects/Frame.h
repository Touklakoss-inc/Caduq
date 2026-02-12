#ifndef CQ_FRAME_H
#define CQ_FRAME_H

#include "Entity.h"

#include <Eigen/Dense>
#include <Eigen/Geometry>

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

        /* ImGui */
        static inline float m_GuiPopupPos[3] { 0.0f, 0.0f, 0.0f };
        static inline float m_GuiPopupRot[4] { 0.0f, 0.0f, 0.0f, 0.0f};
        static inline float m_GuiPopupSca[3] { 1.0f, 1.0f, 1.0f };

    public:
        Frame(Transform transform = Transform::Identity(), Type type = Type::frame, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Delete() override;

        void Translate(Eigen::Vector3d translate);
        void TranslateLocal(Eigen::Vector3d translate);
        void Rotate(Eigen::Quaterniond rotate);
        void RotateLocal(Eigen::Quaterniond rotate);
        void SetPositionRotation(Eigen::Vector3d position, Eigen::Quaterniond rotation);

        void RenderImGui(EntityManager& entityManager) override;
        static void Popup(EntityManager& entityManager);
        static void SetPopupParam(Eigen::Vector3d position, Eigen::Vector4d rotation);

        int GetID() const override { return m_Id; };
        Transform GetTransform() { return m_Transform; };
    };
}
#endif
