#pragma once

#include "Vizir/Renderer/OrthographicCamera.h"
#include "Vizir/Core/Timestep.h"
#include "Vizir/EventSystem/ApplicationEvent.h"

namespace Vizir
{
	class ArcballCameraController
	{
	public:
		ArcballCameraController(float aspectRatio, float screenWidth, float screenHeight);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void ResizeBounds(float width, float height);


		float GetRotationSpeed() const { return m_CameraRotationSpeed; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		OrthographicCamera& GetCamera() { return m_Camera; }

		void SetRotationSpeed(float rotationSpeed) { m_CameraRotationSpeed = rotationSpeed; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		const float m_MinZoomLevel = 0.01f;

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		float m_CameraRotationSpeed = 2.0f; // Feels more natural when rotating

		glm::vec2 m_LastMousePosition = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_LastNormalizedMousePosition = glm::vec2(0.0f, 0.0f);

		bool m_EnableMove = false;

		float m_ScreenWidth = 0.0f, m_ScreenHeight = 0.0f;
	};
}

