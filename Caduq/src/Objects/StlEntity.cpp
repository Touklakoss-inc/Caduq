#include "StlEntity.h"

#include "Objects/Entity.h"
#include "Objects/EntityManager.h"
#include "Vizir/Renderer/VertexArray.h"
#include "Tools/STL.h"

#include <imgui.h>

namespace Caduq
{
    StlEntity::StlEntity(const std::string& filename, const std::shared_ptr<Frame>& frame, Type type, const std::string& name)
        : Entity{ name != "" ? name : "StlEntity " + std::to_string(++s_IdGenerator), type, frame }
        , m_FileName{ filename }
        , m_Mesh{ Tools::ImportSTL(filename) }
    {
    }

    void StlEntity::Init()
    {
        m_Color = glm::vec3{ 1.0, 0.0, 1.0 };
        UpdateGFX();
    }

    void StlEntity::UpdateGFX()
    {
        std::cout << m_Mesh.nodes << '\n';
        std::cout << m_Mesh.elts.transpose() << '\n';
        Entity::UpdateGFXBuffer(m_Mesh.nodes.cast<float>(), m_Mesh.elts.cast<uint32_t>(), Vizir::TRIANGLES);
    }

    void StlEntity::Delete()
    {
        Entity::Delete();
    }

    void StlEntity::RenderImGui(EntityManager& entityManager)
    {
        if (ImGui::TreeNode(m_Name.data()))
        {
            Entity::RenderImGui(entityManager);
            ImGui::TreePop();
        }
    }
}
