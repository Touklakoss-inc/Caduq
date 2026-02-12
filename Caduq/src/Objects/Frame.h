#ifndef CQ_FRAME_H
#define CQ_FRAME_H

#include "Entity.h"
#include "Geometry/Frame.h"

#include "Geometry/Geo.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <memory>
namespace Caduq
{
    class Frame: public Entity
    {
    private:
        static inline int s_IdGenerator{ 0 };
        int m_Id{ };

        Geometry::Frame m_GeoFrame;

        Eigen::Vector3d m_X;
        Eigen::Vector3d m_Y;
        Eigen::Vector3d m_Z;

        /* ImGui */
        static inline float m_GuiPopupPos[3] { 0.0f, 0.0f, 0.0f };
        static inline float m_GuiPopupRot[4] { 0.0f, 0.0f, 0.0f, 0.0f};
        static inline float m_GuiPopupSca[3] { 1.0f, 1.0f, 1.0f };

    public:
        Frame(Geometry::Transform transform, const std::shared_ptr<Frame>& frame, Type type = Type::frame, const std::string& name = "");

        void Init() override;
        void UpdateGFX() override;
        void Update(Eigen::Vector3d position, Eigen::Quaterniond rotation);
        void Update();
        void Delete() override;

        void RenderImGui(EntityManager& entityManager) override;
        static void Popup(EntityManager& entityManager);
        static void SetPopupParam(Eigen::Vector3d position, Eigen::Vector4d rotation);

        int GetID() const override { return m_Id; };
        const Geometry::Frame& GetGeoFrame() { return m_GeoFrame; };
        const Geometry::Transform GetTransform();
    };
}
#endif
