#include "Entity.h"

#include <string>
namespace Caduq 
{
    Entity::Entity(const std::string& name)
        : m_Name{ name }
    {
    }

    void Entity::Init()
    {
    }

    void Entity::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
    }
}
