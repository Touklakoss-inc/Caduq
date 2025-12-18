#include "EntityManager.h"

#include <memory>
namespace Caduq 
{
    void EntityManager::CreateEntity(std::shared_ptr<Entity> entity)
    {
        m_Entity_List.push_back(entity);
        entity->Init();
    }

    std::shared_ptr<Entity> EntityManager::GetEntity(int index)
    {
        return m_Entity_List.at(index);
    }

    int EntityManager::GetUseCount(int index)
    {
        return m_Entity_List.at(index).use_count();
    }
}
