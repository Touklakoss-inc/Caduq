#ifndef GO_FRAME_H
#define GO_FRAME_H

#include "Geo.h"

#include <Eigen/Dense>

namespace Geometry
{
    class Frame
    {
    private:
        Transform m_Transform;

    public:
        Frame(Transform transform = Transform::Identity());

        void Translate(Eigen::Vector3d translate) { m_Transform.pretranslate(translate); };
        void TranslateLocal(Eigen::Vector3d translate) { m_Transform.translate(translate); };
        void Rotate(Eigen::Quaterniond rotate) { m_Transform.prerotate(rotate); };
        void RotateLocal(Eigen::Quaterniond rotate) { m_Transform.rotate(rotate); };

        void SetPositionRotation(Eigen::Vector3d position, Eigen::Quaterniond rotation);

        const Transform GetTransform() const { return m_Transform; };
    };
}
#endif
