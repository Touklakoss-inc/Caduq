#include "Entity.h"

#include "Vizir/Logging/Log.h"
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

    void Entity::AddParent(const std::shared_ptr<Entity>& parent)
    {
        m_Parents.insert(parent);
    }

    void Entity::RemoveParent(const std::shared_ptr<Entity>& parent)
    {
        m_Parents.erase(parent);
    }

    bool Entity::HasParent(const std::shared_ptr<Entity>& parent) const
    {
        return m_Parents.count(parent) == 1;
    }

    void Entity::AddChild(const std::shared_ptr<Entity>& child)
    {
        m_Children.insert(child);
    }

    void Entity::RemoveChild(const std::shared_ptr<Entity>& child)
    {
        m_Children.erase(child);
    }

    bool Entity::HasChild(const std::shared_ptr<Entity>& child) const
    {
        return m_Children.count(child) == 1;
    }
}
