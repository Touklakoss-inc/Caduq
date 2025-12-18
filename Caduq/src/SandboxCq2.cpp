#include "SandboxCq2.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <memory>
void SandboxCq2::OnAttach()
{
	Vizir::RenderCommand::EnablePrimitiveRestart();

    // Shader 
    std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 i_Position;
            
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;
            uniform vec3 u_Color;

            out vec3 v_Color;

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(i_Position, 1.0);
                v_Color = u_Color;
            }
        )";

    std::string fragmentSrc = R"(
            #version 330 core
            
            in vec3 v_Color;

            layout(location = 0) out vec4 color;

            void main()
            {
                color = vec4(v_Color, 1.0);
            }
        )";

    m_Shader = Vizir::Shader::Create("PointShader", vertexSrc, fragmentSrc);


    m_Entity_Manager.CreateEntity(std::make_shared<Caduq::Point>(0, 0, 0));
    m_Entity_Manager.CreateEntity(std::make_shared<Caduq::Point>(1, 0, 0));

    std::cout << m_Entity_Manager.GetEntity(0) << '\n';
    std::cout << m_Entity_Manager.GetEntity(0).use_count() << '\n';
    std::cout << m_Entity_Manager.GetUseCount(0) << '\n';

    Vizir::RenderCommand::SetPointSize(m_PointSize);
	Vizir::RenderCommand::SetLineWidth(m_LineSize);

    // Transform
    m_Transform = glm::mat4(1.0f);
}

void SandboxCq2::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();

    // Render points
    for (auto entity : m_Entity_Manager.GetEntities()) //GetEntities return by value, is it good ?
    {
        entity->Visualize(m_Shader, m_Transform);
    }

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxCq2::OnImGuiRender()
{
    float pointSize = m_PointSize;
    float lineSize = m_LineSize;

    ImGui::Begin("Settings");

    for (auto entity : m_Entity_Manager.GetEntities())
    {
        ImGui::ColorEdit3(static_cast<std::string>(entity->GetName()).data(), glm::value_ptr(entity->GetColor()));
    }

    ImGui::DragFloat("Point Size", &pointSize, 1.0f, 1.0f, 25.0f);
    ImGui::DragFloat("Line Size", &lineSize, 1.0f, 1.0f, 10.0f);

    ImGui::End();

    if (pointSize != m_PointSize)
    {
        m_PointSize = pointSize;
        Vizir::RenderCommand::SetPointSize(m_PointSize);
    }
    if (lineSize != m_LineSize)
    {
        m_LineSize = lineSize;
        Vizir::RenderCommand::SetLineWidth(m_LineSize);
    }
    
    // ImGui::ShowDemoWindow();
}

void SandboxCq2::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
