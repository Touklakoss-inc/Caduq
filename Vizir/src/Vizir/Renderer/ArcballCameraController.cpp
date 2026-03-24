#include "vzpch.h"
#include "ArcballCameraController.h"

#include "Core/KeyCodes.h"
#include "Core/MouseButtonCode.h"
#include "Core/Input.h"

namespace Vizir
{
  ArcballCameraController::ArcballCameraController(float aspectRatio, float screenWidth, float screenHeight)
    : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
  {
  }

  void ArcballCameraController::OnUpdate(Timestep ts)
  {
    BOB_PROFILE_FUNC()

    // When the middle mouse button is not clicked, disable movement
    // This also forces to reevaluate the last mouse position (see bottom of the method) 
    // before actually modifying the camera.
    if (!Input::IsMouseButtonPressed(VZ_MOUSE_BUTTON_MIDDLE))
    {
      m_EnableMove = false;
      return;
    }

    glm::vec2 mousePosition = Input::GetMousePosition();
    // invert mouse coordinates y axis to be coherent with the world y axis
    mousePosition.y *= -1.0f;
    // Normalize mouse position to [-1.0, 1.0]
    glm::vec2 normalizedMousePosition = glm::vec2(2.0f * mousePosition.x / m_ScreenWidth - 1.0f, 2.0f * mousePosition.y / m_ScreenHeight + 1.0f);

    glm::vec2 displacement = mousePosition - m_LastMousePosition;

    if (m_EnableMove && (displacement.x * displacement.x > 0.0f || displacement.y * displacement.y > 0.0f))
    {
      if (Input::IsMouseButtonPressed(VZ_MOUSE_BUTTON_RIGHT))
      {
        // Project the mouse position onto the arcball (a "unit sphere") by computing z from x and y coordinates and sphere equation
        glm::vec3 lastArcballProjection = glm::vec3(m_LastNormalizedMousePosition, 0.0f);
        glm::vec3 arcballProjection = glm::vec3(normalizedMousePosition, 0.0f);

        lastArcballProjection.z = glm::sqrt(1.0f - m_LastNormalizedMousePosition.y * m_LastNormalizedMousePosition.y - m_LastNormalizedMousePosition.x * m_LastNormalizedMousePosition.x);
        lastArcballProjection /= glm::length(lastArcballProjection);

        arcballProjection.z = glm::sqrt(1.0f - normalizedMousePosition.y * normalizedMousePosition.y - normalizedMousePosition.x * normalizedMousePosition.x);
        arcballProjection /= glm::length(arcballProjection);

        // Compute the angle between both arcball projections vectors in the sphere basis
        float dotProd = glm::dot(lastArcballProjection, arcballProjection);

        if (dotProd < 1.0f)
        {
          float angle = glm::acos(dotProd);
          // Compute the axis around which we will rotate
          glm::vec3 axis = glm::normalize(glm::cross(arcballProjection, lastArcballProjection));

          // Multiplying the angle by 2 feels way more natural
          m_Camera.Rotate(2.0f * angle, axis);
        }
      }
      else
      {
        // Displacement mode
        // We want to map the mouse positions [0, screenWidth] x [0, screenHeight] 
        // to the projection bounds size [0, 2.0f * m_ScreenWidth / m_ScreenHeight * m_ZoomLevel] x [0, 2.0 * m_ZoomLevel]
        // So we just need to multiply both intervals by 2.0f / m_ScreenHeight * m_ZoomLevel
        float scale = 2.0f * m_ZoomLevel / m_ScreenHeight;
        glm::vec3 translation = glm::vec3(-displacement * scale, 0.0f);
        m_Camera.Translate(translation);
      }
    }

    m_LastMousePosition = mousePosition;
    m_LastNormalizedMousePosition = normalizedMousePosition;
    m_EnableMove = true;
  }

  void ArcballCameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ArcballCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(ArcballCameraController::OnWindowResized));
  }

  void ArcballCameraController::ResizeBounds(float width, float height)
  {
    m_AspectRatio = width / height;
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
  }

  bool ArcballCameraController::OnMouseScrolled(MouseScrolledEvent& e)
  {
    m_ZoomLevel -= (float)e.GetYOffset();

    if (m_ZoomLevel < m_MinZoomLevel)
    {
      m_ZoomLevel = m_MinZoomLevel;
    }

    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

    return true;
  }

  bool ArcballCameraController::OnWindowResized(WindowResizedEvent& e)
  {
    m_ScreenWidth = (float)e.GetWidth();
    m_ScreenHeight = (float)e.GetHeight();
    ResizeBounds(m_ScreenWidth, m_ScreenHeight);

    return true;
  }
}
