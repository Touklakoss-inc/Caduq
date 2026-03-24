#include "cqpch.h"
#include "SandboxCube.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "Vizir/Renderer/Shader.h"

SandboxCube::SandboxCube()
	: Layer("Sandbox - Cube"), m_CameraController(1280.0f / 720.0f, 1280.0f, 720.0f)
{
}

void SandboxCube::OnAttach()
{
	m_CameraController.GetCamera().Translate(glm::vec3(0.0f, 0.0f, 10.0f));

	// Visualization buffer

	// Vertex Buffer
	float vertices[6 * 4 * 6] = {
		-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,

		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	};

	Vizir::Ref<Vizir::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Vizir::VertexBuffer::Create(vertices, sizeof(vertices)));

	Vizir::BufferLayout layout = {
		{ Vizir::ShaderDataType::Float3, "a_Position"},
		{ Vizir::ShaderDataType::Float3, "a_Color"},
	};
	m_VertexBuffer->SetLayout(layout);

	// Index Buffer
	unsigned int indices[6 * 6] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15, 

		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	Vizir::Ref<Vizir::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Vizir::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	// Vertex array
	m_VertexArray = Vizir::VertexArray::Create();
	m_VertexArray->Bind();
	m_VertexArray->SetVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->SetPrimitiveType(Vizir::PrimitiveType::TRIANGLES);
	m_VertexArray->Unbind();

	// Shader
	m_ShaderLibrary.Load("Color", "src/Caduq/Assets/shaders/color.glsl");
}

void SandboxCube::OnImGuiRender()
{

}

void SandboxCube::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

	auto textureShader = m_ShaderLibrary.Get("Color");
	Vizir::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Vizir::Renderer::EndScene();
}

void SandboxCube::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
