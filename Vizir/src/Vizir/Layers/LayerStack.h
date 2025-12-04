#pragma once

#include "Vizir/Layers/Layer.h"

#include "Vizir/Core/Core.h"

namespace Vizir
{
	class VZ_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(const Ref<Layer>& layer);
		void PushOverlay(const Ref<Layer>& overlay);
		void PopLayer(const Ref<Layer>& layer);
		void PopOverlay(const Ref<Layer>& overlay);

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

