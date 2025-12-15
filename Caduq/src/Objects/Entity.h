#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <string_view>

namespace Caduq 
{
    class Entity
    {
        private:
            std::string m_Name{};

        public:
            Entity(const std::string& name);
            std::string_view GetName();
    };
}

#endif
