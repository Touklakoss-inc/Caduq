#ifndef ENTITY_H
#define ENTITY_H

#include "Vizir.h"

#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <unordered_set>

namespace Caduq 
{
    enum class Type
    {
        point,
        spline,
        patch
    };

    class EntityManager;

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

        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> m_Parents;

    protected:
        std::string m_Name {};
        glm::vec3 m_Color { 1.0f, 0.0f, 0.0f };
        float m_Size { 5.0f };
        Type m_Type { };
        bool m_deleting { false };

        Vizir::Ref<Vizir::VertexArray> m_VertexArray;
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> m_Children;

    public:
        Entity(const std::string& name, Type type);

        virtual void Init() = 0;
        virtual void UpdateGFX() = 0;
        void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform);
        virtual void RenderImGui(EntityManager& entityManager);

        void AddParent(const std::shared_ptr<Entity>& parent);
        void RemoveParent(const std::shared_ptr<Entity>& parent);
        bool HasParent(const std::shared_ptr<Entity>& parent) const;
        void ClearParents() { m_Parents.clear(); };

        void AddChild(const std::shared_ptr<Entity>& child);
        void RemoveChild(const std::shared_ptr<Entity>& child);
        bool HasChild(const std::shared_ptr<Entity>& child) const;
        void ClearChildren() { m_Children.clear(); };

        const std::string& GetName() { return m_Name; };
        virtual int GetID() const = 0;
        Type GetType() { return m_Type; };
        bool GetDeletingStatus() { return m_deleting; };
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> GetParents() { return m_Parents; };
        std::unordered_set<std::shared_ptr<Entity>, SharedPtrHash, SharedPtrComparator> GetChildren() { return m_Children; };

        virtual void Delete();

        virtual ~Entity();
    };

}

#endif
