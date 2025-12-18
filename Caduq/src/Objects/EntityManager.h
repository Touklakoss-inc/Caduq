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
            std::shared_ptr<Entity> GetEntity(int index);
            int GetUseCount(int index);
            // void DeleteEntity();
    };
}


#endif
