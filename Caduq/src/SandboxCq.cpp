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
    cp2.Init();
    cp3.Init();

    std::cout << cp0.GetName() << '\n';
    std::cout << cp1.GetName() << '\n';
    std::cout << cp2.GetName() << '\n';
    std::cout << cp3.GetName() << '\n';

    cs0.Init();
    cs1.Init();
    cs2.Init();
    cs3.Init();

    std::cout << cs0.GetName() << '\n';
    std::cout << cs1.GetName() << '\n';
    std::cout << cs2.GetName() << '\n';
    std::cout << cs3.GetName() << '\n';

    cc0.Init();

    Vizir::RenderCommand::SetPointSize(m_PointSize);
	Vizir::RenderCommand::SetLineWidth(m_LineSize);

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

    // Render patches
    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);

    cc0.Visualize(m_Shader, m_Transform);

    // Render splines
    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_LineColor);

    cs0.Visualize(m_Shader, m_Transform);
    cs1.Visualize(m_Shader, m_Transform);
    cs2.Visualize(m_Shader, m_Transform);
    cs3.Visualize(m_Shader, m_Transform);

    // Render points
    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_PointColor);

    cp0.Visualize(m_Shader, m_Transform);
    cp1.Visualize(m_Shader, m_Transform);
    cp2.Visualize(m_Shader, m_Transform);
    cp3.Visualize(m_Shader, m_Transform);

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxCq::OnImGuiRender()
{
    float pointSize = m_PointSize;
    float lineSize = m_LineSize;

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("Point Color", glm::value_ptr(m_PointColor));
    ImGui::DragFloat("Point Size", &pointSize, 1.0f, 1.0f, 25.0f);

    ImGui::ColorEdit3("Spline Color", glm::value_ptr(m_LineColor));
    ImGui::DragFloat("Line Size", &lineSize, 1.0f, 1.0f, 10.0f);

    ImGui::ColorEdit3("Patch Color", glm::value_ptr(m_TriangleColor));

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

void SandboxCq::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
