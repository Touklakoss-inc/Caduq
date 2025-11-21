#pragma once

#include "Vizir/Renderer/OrthographicCamera.h"
#include "Vizir/Core/Timestep.h"

#include "Vizir/EventSystem/ApplicationEvent.h"

namespace Vizir
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const OrthographicCamera& GetCamera() const { return m_Camera; }
		OrthographicCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
		void RecomputeSpeed();
	private:
		const float m_MinZoomLevel = 0.01f;
		const float m_BaseCameraTranslationSpeed = 1.0f;

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = m_BaseCameraTranslationSpeed, m_CameraRotationSpeed = 1.0f;
	};
}

