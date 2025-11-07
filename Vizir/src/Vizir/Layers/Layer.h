#pragma once

#include "Vizir/EventSystem/Event.h"

#include "Vizir/Core/Core.h"
#include "Vizir/Core/Timestep.h"

namespace Vizir
{
	class VZ_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}


