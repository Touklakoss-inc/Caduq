#ifndef ENTITY_H
#define ENTITY_H

#include "glm/fwd.hpp"
#include <Vizir.h>
#include <memory>
#include <string>
#include <string_view>

#include <unordered_set>
namespace Caduq 
{
    class Entity : public std::enable_shared_from_this<Entity>
    {
    private:
        struct SharedPtrHash
        {
            size_t operator()(const std::shared_ptr<Entity>& ptr) const
            {
                return ptr ? std::hash<Entity*>{}(ptr.get()) : 0; // hash made with the adress of the objects, use a unique ID instead ?
            }
        };

        struct SharedPtrComparator
        {
            bool operator()(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) const
            {
                return a.get() == b.get();
            }
        };

        std::string m_Name {};
        glm::vec3 m_Color { 1.0f, 0.0f, 0.0f };
        float m_Size { 5.0f };

        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> m_Parents;
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> m_Children;

    public:
        Entity(const std::string& name);

        virtual void Init();
        virtual void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform);
        void RenderImGui();

        void AddParent(const std::shared_ptr<Entity>& parent);
        void RemoveParent(const std::shared_ptr<Entity>& parent);
        bool HasParent(const std::shared_ptr<Entity>& parent) const;

        void AddChild(const std::shared_ptr<Entity>& child);
        void RemoveChild(const std::shared_ptr<Entity>& child);
        bool HasChild(const std::shared_ptr<Entity>& child) const;

        std::string_view GetName() { return m_Name; };
        glm::vec3& GetColor() { return m_Color; };
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> GetParents() { return m_Parents; };
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> GetChildren() { return m_Children; };

        bool Delete();

        virtual ~Entity() = default;
    };

}

#endif
