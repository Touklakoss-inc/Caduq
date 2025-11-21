#include "SandboxCq.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

void SandboxCq::OnAttach()
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

    cp0.Init();
    cp1.Init();

    Vizir::RenderCommand::SetPointSize(m_PointSize);

    // Transform
    m_Transform = glm::mat4(1.0f);
}

void SandboxCq::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();
    // Render points
    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_PointColor);

    cp0.Visualize(m_Shader, m_Transform);
    cp1.Visualize(m_Shader, m_Transform);

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxCq::OnImGuiRender()
{
    float pointSize = m_PointSize;

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("Point Color", glm::value_ptr(m_PointColor));
    ImGui::DragFloat("Point Size", &pointSize, 1.0f, 1.0f, 25.0f);

    ImGui::End();

    if (pointSize != m_PointSize)
    {
        m_PointSize = pointSize;
        Vizir::RenderCommand::SetPointSize(m_PointSize);
    }
}

void SandboxCq::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
