#ifndef ENTITY_H
#define ENTITY_H

#include "glm/fwd.hpp"
#include <Vizir.h>
#include <string>
#include <string_view>

namespace Caduq 
{
    class Entity
    {
    private:
        std::string m_Name {};
        glm::vec3 m_Color { 1.0f, 0.0f, 0.0f };
        float m_Size { 5.0f };

    public:
        Entity(const std::string& name);

        virtual void Init();
        virtual void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform);

        std::string_view GetName() { return m_Name; };
        glm::vec3& GetColor() { return m_Color; };

        virtual ~Entity() = default;
    };
}

#endif
