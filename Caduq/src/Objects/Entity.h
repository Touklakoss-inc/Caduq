#ifndef ENTITY_H
#define ENTITY_H

#include <Vizir.h>
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

            virtual void Init();
            virtual void Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform);

            virtual ~Entity() = default;
    };
}

#endif
