#include "SandboxSplines.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

void SandboxSplines::OnAttach()
{
	// Geometry 
	const int MESH_SIZE{ 100 };

	Eigen::MatrixXd U{ 4, MESH_SIZE };
	Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0) };

	U.row(0) = Eigen::VectorXd::Ones(MESH_SIZE);
	U.row(1) = u;
	U.row(2) = u * u;
	U.row(3) = u * u * u;
	
	// Get data and extract buffer

	// points
	Eigen::MatrixXf points = s0.Mesh(U).cast<float>();

	// points indices
	Eigen::ArrayX<uint32_t> indices{ Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE, 0, MESH_SIZE - 1) };

	// Vertex buffer
	//float vertices[3 * 3] /*[3 * 7]*/ = {
	//		-0.5f, -0.5f, 0.0f, // 1.0f, 0.0f, 0.0f, 1.0f,
	//		 0.5f, -0.5f, 0.0f, // 0.0f, 1.0f, 0.0f, 1.0,
	//		 0.0f,  0.5f, 0.0f, // 0.0f, 0.0f, 1.0f, 1.0f
	//};

	Vizir::Ref<Vizir::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Vizir::VertexBuffer::Create(points.data(), points.size() * sizeof(float)));

	Vizir::BufferLayout layout = {
		{ Vizir::ShaderDataType::Float3, "v_position"},
	};
	vertexBuffer->SetLayout(layout);

	// ---- Points ----
	// Point Index buffer
	Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
	pointIndexBuffer.reset(Vizir::IndexBuffer::Create(indices.data(), indices.size()));

	// Point Vertex array
	m_PointVertexArray = Vizir::VertexArray::Create();
	m_PointVertexArray->SetVertexBuffer(vertexBuffer);
	m_PointVertexArray->SetIndexBuffer(pointIndexBuffer);
	m_PointVertexArray->SetPrimitiveType(Vizir::POINTS);
	m_PointVertexArray->Unbind();

	// ---- Lines ----
	// Line Index buffer

	Vizir::Ref<Vizir::IndexBuffer> lineIndexBuffer;
	lineIndexBuffer.reset(Vizir::IndexBuffer::Create(indices.data(), indices.size()));

	// Line Vertex array
	m_LineVertexArray = Vizir::VertexArray::Create();
	m_LineVertexArray->SetVertexBuffer(vertexBuffer);
	m_LineVertexArray->SetIndexBuffer(lineIndexBuffer);
	m_LineVertexArray->SetPrimitiveType(Vizir::LINE_STRIP);
	m_LineVertexArray->Unbind();

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

	// Transform
	m_Transform = glm::mat4(1.0f);

	// Update attributes
	Vizir::RenderCommand::SetPointSize(m_PointSize);
	Vizir::RenderCommand::SetLineWidth(m_LineWidth);
}

void SandboxSplines::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();

	// Render lines
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_LineColor);
	Vizir::Renderer::Submit(m_Shader, m_LineVertexArray, m_Transform);

	// Render points
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_PointColor);
	Vizir::Renderer::Submit(m_Shader, m_PointVertexArray, m_Transform);

	Vizir::Renderer::EndScene();
}

void SandboxSplines::OnImGuiRender()
{
	float pointSize = m_PointSize;
	float lineSize = m_LineWidth;

	ImGui::Begin("Settings");

	ImGui::ColorEdit3("Point Color", glm::value_ptr(m_PointColor));
	ImGui::DragFloat("Point Size", &pointSize, 1.0f, 1.0f, 25.0f);

	ImGui::ColorEdit3("Lines Color", glm::value_ptr(m_LineColor));
	ImGui::DragFloat("Lines Size", &lineSize, 1.0f, 1.0f, 10.0f);

	ImGui::End();

	if (pointSize != m_PointSize)
	{
		m_PointSize = pointSize;
		Vizir::RenderCommand::SetPointSize(m_PointSize);
	}
	if (lineSize != m_LineWidth)
	{
		m_LineWidth = lineSize;
		Vizir::RenderCommand::SetLineWidth(m_LineWidth);
	}
}

void SandboxSplines::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}