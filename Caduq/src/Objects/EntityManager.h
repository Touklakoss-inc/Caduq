#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"

#include <vector>
#include <memory>

namespace Caduq 
{
    class EntityManager
    {
        private:
            std::vector<std::shared_ptr<Entity>> m_Entity_List {};

        public:
            EntityManager() = default;

            void CreateEntity(std::shared_ptr<Entity> entity);
            // void DeleteEntity();


            std::vector<std::shared_ptr<Entity>> GetEntities() { return m_Entity_List; }; // should it be returned by reference ?
            std::shared_ptr<Entity> GetEntity(int index);
            template<typename T> 
            std::shared_ptr<T> GetEntity(int index);
            int GetUseCount(int index);
    };

    template<typename T> 
    std::shared_ptr<T> EntityManager::GetEntity(int index)
    {
        return std::dynamic_pointer_cast<T>(m_Entity_List.at(index)); // perform a check for nullptr ?
    }

}


#endif
