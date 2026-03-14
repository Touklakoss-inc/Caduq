#include "vzpch.h"
#include "OrthographicCameraController.h"

#include "Core/Core.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Vizir
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation = false)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {

    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        BOB_PROFILE_FUNC()

        glm::vec3 displacement{ 0.0f, 0.0f, 0.0f };
        if (Input::IsKeyPressed(VZ_KEY_W))
            displacement.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(VZ_KEY_S))
            displacement.y -= m_CameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(VZ_KEY_A))
            displacement.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(VZ_KEY_D))
            displacement.x += m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            float angle = 0.0f;

            if (Input::IsKeyPressed(VZ_KEY_Q))
                angle  = m_CameraRotationSpeed * ts;
            else if (Input::IsKeyPressed(VZ_KEY_E))
                angle = -m_CameraRotationSpeed * ts;

            m_Camera.Rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        }

        m_Camera.Translate(displacement);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::ResizeBounds(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        RecomputeSpeed();
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
      m_ZoomLevel -= (float)e.GetYOffset();

      if (m_ZoomLevel < m_MinZoomLevel)
      {
        m_ZoomLevel = m_MinZoomLevel;
      }

      m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
      RecomputeSpeed();

      return true;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
    {
      ResizeBounds((float)e.GetWidth(), (float)e.GetHeight());

      return true;
    }

    void OrthographicCameraController::RecomputeSpeed()
    {
      m_CameraTranslationSpeed = 2.0f * m_ZoomLevel * m_BaseCameraTranslationSpeed;
    }
}
