#pragma once

#include "Core.h"
#include <glm/glm.hpp>

namespace Vizir
{
	class VZ_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
