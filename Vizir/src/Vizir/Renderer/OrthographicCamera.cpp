#include "vzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vizir
{
	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(glm::mat4(1.0f))
	{
		BOB_PROFILE_FUNC()

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float top, float bottom)
	{
		BOB_PROFILE_FUNC()

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::Rotate(float angle, const glm::vec3& axis)
	{
		BOB_PROFILE_FUNC()

		m_RotationQuat = glm::rotate(m_RotationQuat, angle, axis);
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		BOB_PROFILE_FUNC()

		glm::mat4 transform = glm::mat4_cast(m_RotationQuat) * glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
