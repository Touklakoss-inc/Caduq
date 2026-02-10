#include "SandboxFramebuffer.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "Vizir/Renderer/Shader.h"

#include <imgui.h>
#include <glm/ext.hpp>


SandboxFramebuffer::SandboxFramebuffer()
	: Layer("Sandbox - Framebuffer"), m_CameraController(1280.0f / 720.0f, true)
{
}

void SandboxFramebuffer::OnAttach()
{
	// --------------------------------------------------------------------------------------
	// Scene Setup
	// --------------------------------------------------------------------------------------
	// 
	// Visualization buffer

	// Vertex Buffer
	float vertices[5 * 4] = {
		-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
		 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
		 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
		-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
	};

	Vizir::Ref<Vizir::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Vizir::VertexBuffer::Create(vertices, sizeof(vertices)));

	Vizir::BufferLayout layout = {
		{ Vizir::ShaderDataType::Float3, "a_Position"},
		{ Vizir::ShaderDataType::Float2, "a_TexCoords"},
	};
	m_VertexBuffer->SetLayout(layout);

	// Index Buffer
	unsigned int indices[4] = {
		0, 1, 2, 3
	};

	Vizir::Ref<Vizir::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Vizir::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	// Vertex array
	m_VertexArray = Vizir::VertexArray::Create();
	m_VertexArray->Bind();
	m_VertexArray->SetVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->SetPrimitiveType(Vizir::PrimitiveType::TRIANGLE_FAN);
	m_VertexArray->Unbind();

	// Shader
	auto texture = m_ShaderLibrary.Load("Texture", "src/Assets/shaders/texture.glsl");

	// Use files as shaders
	m_GridTexture = Vizir::Texture2D::Create("src/Assets/textures/texture.png");
	m_LogoTexture = Vizir::Texture2D::Create("src/Assets/textures/ChernoLogo.png");

	// For sanity, set texture unit used to 0
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(texture)->Bind();
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(texture)->UploadUniformInt("u_Texture", 0);

	// --------------------------------------------------------------------------------------
	// Framebuffer setup
	// --------------------------------------------------------------------------------------

	m_Framebuffer = Vizir::Framebuffer::Create({ static_cast<uint32_t>(m_WindowWidth), static_cast<uint32_t>(m_WindowHeight) });
}

void SandboxFramebuffer::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin(GetName().c_str());
	ImVec2 size = ImGui::GetContentRegionAvail();
	if (m_WindowWidth != size.x || m_WindowHeight != size.y)
	{
		m_WindowWidth = size.x;
		m_WindowHeight = size.y;

		m_Framebuffer->Resize(static_cast<uint32_t>(m_WindowWidth), static_cast<uint32_t>(m_WindowHeight));
		m_CameraController.ResizeBounds(m_WindowWidth, m_WindowHeight);
	}

	ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(m_Framebuffer->GetAttachmentNativeID())), ImVec2(m_WindowWidth, m_WindowHeight), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();
}

void SandboxFramebuffer::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// Clean window framebuffer
	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	// Framebuffer binding
	m_Framebuffer->Bind();

	// Clean texture framebuffer
	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

	// Scene rendering

	auto textureShader = m_ShaderLibrary.Get("Texture");
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(textureShader)->Bind();

	// Setup texture parameters
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(textureShader)->UploadUniformFloat("u_TilingFactor", m_GridTilingFactor);
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(textureShader)->UploadUniformFloat4("u_Color", glm::vec4(m_GridColor, 1.0));

	m_GridTexture->Bind(0);
	Vizir::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Setup texture parameters
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(textureShader)->UploadUniformFloat("u_TilingFactor", m_LogoTilingFactor);
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(textureShader)->UploadUniformFloat4("u_Color", glm::vec4(m_LogoColor, 1.0));

	m_LogoTexture->Bind(0);
	Vizir::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Framebuffer unbinding
	m_Framebuffer->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxFramebuffer::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
