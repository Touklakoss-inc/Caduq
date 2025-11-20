#pragma once

#include <glm/glm.hpp>

namespace Vizir
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float top, float bottom);

		void SetProjection(float left, float right, float top, float bottom);

		const glm::vec3 GetPosition() const { return m_Position; }
		const float GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }


		void Translate(const glm::vec3& translation) { SetPosition(m_Position + translation); }
		void Rotate(float rotation) { SetRotation(m_Rotation + rotation); }

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}
