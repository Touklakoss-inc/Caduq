#include "SandboxSplines.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

void SandboxSplines::OnAttach()
{
	Vizir::RenderCommand::EnablePrimitiveRestart();

	// Geometry 
	Geometry::Point p0{ 0.0, 0.0, 0.0 };
	Geometry::Point p1{ 3.0, 0.0, 1.0 };
	Geometry::Point p2{ 3.0, 4.0, 0.0 };
	Geometry::Point p3{ 0.0, 4.0, 1.0 };

	Geometry::SplinePoint sp0{ p0, { 1.0, 0.0, 0.0 }, 1.0 };
	Geometry::SplinePoint sp1{ p1, { 1.0, 0.0, 0.0 }, 1.0 };
	Geometry::Spline s0{ sp0, sp1 };

	Geometry::SplinePoint sp2{ p2, { 0.0, -1.0, 0.0 }, 1.0 };
	Geometry::SplinePoint sp3{ p1, { 0.0, -1.0, 0.0 }, 1.0 };
	Geometry::Spline s1{ sp2, sp3 };

	Geometry::SplinePoint sp4{ p3, { 1.0, 0.0, 0.0 }, 1.0 };
	Geometry::SplinePoint sp5{ p2, { 1.0, 0.0, 0.0 }, 1.0 };
	Geometry::Spline s2{ sp4, sp5 };

	Geometry::SplinePoint sp6{ p3, { 0.0, -1.0, 0.0 }, 1.0 };
	Geometry::SplinePoint sp7{ p0, { 0.0, -1.0, 0.0 }, 1.0 };
	Geometry::Spline s3{ sp6, sp7 };

	const int MESH_SIZE{ 10 };
	Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0) };
	Eigen::MatrixXd U0 = s0.Mesh(u, MESH_SIZE);
	Eigen::MatrixXd U1 = s1.Mesh(u, MESH_SIZE);
	Eigen::MatrixXd U2 = s2.Mesh(u, MESH_SIZE);
	Eigen::MatrixXd U3 = s3.Mesh(u, MESH_SIZE);

	Geometry::Patch c0{ s2, s0, s3, s1 };
	Eigen::MatrixXd m = c0.Mesh(u, u, MESH_SIZE);
	
	// Prepare data for visualization

	// --- Points ---

	// Gather points
	std::vector<float> pointsVertices(4 * 3);

	// Cast to float
	Eigen::Vector3f p0f = p0.GetPosition().cast<float>();
	Eigen::Vector3f p1f = p1.GetPosition().cast<float>();
	Eigen::Vector3f p2f = p2.GetPosition().cast<float>();
	Eigen::Vector3f p3f = p3.GetPosition().cast<float>();

	// copy data
	memcpy(pointsVertices.data() + 0 * 3, p0f.data(), 3 * sizeof(float));
	memcpy(pointsVertices.data() + 1 * 3, p1f.data(), 3 * sizeof(float));
	memcpy(pointsVertices.data() + 2 * 3, p2f.data(), 3 * sizeof(float));
	memcpy(pointsVertices.data() + 3 * 3, p3f.data(), 3 * sizeof(float));

	// --- Splines ---
	// Points to float
	Eigen::MatrixXf splinePoints0 = U0.cast<float>();
	Eigen::MatrixXf splinePoints1 = U1.cast<float>();
	Eigen::MatrixXf splinePoints2 = U2.cast<float>();
	Eigen::MatrixXf splinePoints3 = U3.cast<float>();

	// Extract Splines indices
	std::vector<float> splineVertices(4 * 3 * MESH_SIZE);
	memcpy(splineVertices.data() + 0 * 3 * MESH_SIZE, splinePoints0.data(), MESH_SIZE * 3 * sizeof(float));
	memcpy(splineVertices.data() + 1 * 3 * MESH_SIZE, splinePoints1.data(), MESH_SIZE * 3 * sizeof(float));
	memcpy(splineVertices.data() + 2 * 3 * MESH_SIZE, splinePoints2.data(), MESH_SIZE * 3 * sizeof(float));
	memcpy(splineVertices.data() + 3 * 3 * MESH_SIZE, splinePoints3.data(), MESH_SIZE * 3 * sizeof(float));

	// --- Patch ---
	// Points to float
	Geometry::Mesh mesh = c0.GetGfxMesh();
	Eigen::MatrixXf patchVertices = mesh.nodes.cast<float>();
	Eigen::VectorX<uint32_t> patchIndices = mesh.elts.cast<uint32_t>();

	// Visualization buffer

	// --- Points ---

	// Vertex Buffer
	Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
	pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(pointsVertices.data(), pointsVertices.size() * sizeof(float)));

	Vizir::BufferLayout pointsLayout = {
		{ Vizir::ShaderDataType::Float3, "v_position"},
	};
	pointsVertexBuffer->SetLayout(pointsLayout);

	// Index buffer
	uint32_t pointIndices[4] = {
		0, 1, 2, 3
	};
	Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
	pointIndexBuffer.reset(Vizir::IndexBuffer::Create(pointIndices, sizeof(pointIndices) / sizeof(uint32_t)));

	// Vertex array
	m_PointVertexArray = Vizir::VertexArray::Create();
	m_PointVertexArray->SetVertexBuffer(pointsVertexBuffer);
	m_PointVertexArray->SetIndexBuffer(pointIndexBuffer);
	m_PointVertexArray->SetPrimitiveType(Vizir::POINTS);
	m_PointVertexArray->Unbind();

	// --- Splines ---

	// Vertex Buffer
	Vizir::Ref<Vizir::VertexBuffer> splinesVertexBuffer;
	splinesVertexBuffer.reset(Vizir::VertexBuffer::Create(splineVertices.data(), splineVertices.size() * sizeof(float)));

	Vizir::BufferLayout splinesLayout = {
		{ Vizir::ShaderDataType::Float3, "v_position"},
	};
	splinesVertexBuffer->SetLayout(splinesLayout);

	// Index buffer
	// vertex n * MESH_SIZE - 1 (n = 0, .., 3) is duplicated
	// needs to be addressed !
	std::vector<uint32_t> splinesIndices(4 * (MESH_SIZE + 1));
	{
			Eigen::ArrayX<uint32_t> spline0 = Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE + 1, 0 * MESH_SIZE, MESH_SIZE);
			spline0[MESH_SIZE] = UINT32_MAX;
			Eigen::ArrayX<uint32_t> spline1 = Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE + 1, 1 * MESH_SIZE, 2 * MESH_SIZE);
			spline1[MESH_SIZE] = UINT32_MAX;
			Eigen::ArrayX<uint32_t> spline2 = Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE + 1, 2 * MESH_SIZE, 3 * MESH_SIZE);
			spline2[MESH_SIZE] = UINT32_MAX;
			Eigen::ArrayX<uint32_t> spline3 = Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE + 1, 3 * MESH_SIZE, 4 * MESH_SIZE);
			spline3[MESH_SIZE] = UINT32_MAX;

			memcpy(splinesIndices.data() + 0 * (MESH_SIZE + 1), spline0.data(), (MESH_SIZE + 1) * sizeof(uint32_t));
			memcpy(splinesIndices.data() + 1 * (MESH_SIZE + 1), spline1.data(), (MESH_SIZE + 1) * sizeof(uint32_t));
			memcpy(splinesIndices.data() + 2 * (MESH_SIZE + 1), spline2.data(), (MESH_SIZE + 1) * sizeof(uint32_t));
			memcpy(splinesIndices.data() + 3 * (MESH_SIZE + 1), spline3.data(), (MESH_SIZE + 1) * sizeof(uint32_t));
	}

	Vizir::Ref<Vizir::IndexBuffer> splinesIndexBuffer;
	splinesIndexBuffer.reset(Vizir::IndexBuffer::Create(splinesIndices.data(), splinesIndices.size()));

	// Vertex array
	m_SplineVertexArray = Vizir::VertexArray::Create();
	m_SplineVertexArray->SetVertexBuffer(splinesVertexBuffer);
	m_SplineVertexArray->SetIndexBuffer(splinesIndexBuffer);
	m_SplineVertexArray->SetPrimitiveType(Vizir::LINE_STRIP);
	m_SplineVertexArray->Unbind();

	// --- Patch ---
	// Vertex Buffer
	Vizir::Ref<Vizir::VertexBuffer> patchVertexBuffer;
	patchVertexBuffer.reset(Vizir::VertexBuffer::Create(patchVertices.data(), patchVertices.size() * sizeof(float)));

	Vizir::BufferLayout patchLayout = {
		{ Vizir::ShaderDataType::Float3, "v_position"},
	};
	patchVertexBuffer->SetLayout(patchLayout);

	// Index buffer
	Vizir::Ref<Vizir::IndexBuffer> patchIndexBuffer;
	patchIndexBuffer.reset(Vizir::IndexBuffer::Create(patchIndices.data(), patchIndices.size()));

	// Vertex array
	m_PatchVertexArray = Vizir::VertexArray::Create();
	m_PatchVertexArray->SetVertexBuffer(patchVertexBuffer);
	m_PatchVertexArray->SetIndexBuffer(patchIndexBuffer);
	m_PatchVertexArray->SetPrimitiveType(Vizir::TRIANGLES);
	m_PatchVertexArray->Unbind();

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

	// Render patch
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);
	Vizir::Renderer::Submit(m_Shader, m_PatchVertexArray, m_Transform);

	// Render lines
	std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_LineColor);
	Vizir::Renderer::Submit(m_Shader, m_SplineVertexArray, m_Transform);

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

	ImGui::ColorEdit3("Line Color", glm::value_ptr(m_LineColor));
	ImGui::DragFloat("Line Size", &lineSize, 1.0f, 1.0f, 10.0f);

	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));

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
