#include "vzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vizir
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		BOB_PROFILE_FUNC()

		SetProjection(left, right, bottom, top);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		BOB_PROFILE_FUNC()

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_Near, m_Far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::Rotate(float angle, const glm::vec3& axis)
	{
		BOB_PROFILE_FUNC()

		m_RotationQuat = glm::rotate(m_RotationQuat, angle, axis);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		BOB_PROFILE_FUNC()

		glm::mat4 transform = glm::mat4_cast(m_RotationQuat) * glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
