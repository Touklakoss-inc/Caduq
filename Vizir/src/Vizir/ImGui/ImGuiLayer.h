#pragma once

#include "Vizir/Core/Core.h"

#include "Vizir/Layers/Layer.h"

#include "Vizir/EventSystem/Event.h"
#include "Vizir/EventSystem/ApplicationEvent.h"

namespace Vizir
{
	class VZ_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
