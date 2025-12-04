#include "vzpch.h"
#include "LayerStack.h"

namespace Vizir
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		VZ_PROFILE_FUNC();

		for (auto layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerStack::PushLayer(const Ref<Layer>& layer)
	{
		VZ_PROFILE_FUNC()

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		layer->OnAttach();

		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(const Ref<Layer>& overlay)
	{
		VZ_PROFILE_FUNC()

		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(const Ref<Layer>& layer)
	{
		VZ_PROFILE_FUNC()

		auto it = std::find(begin(), end(), layer);
		if (it != end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetach();
		}
	}
	void LayerStack::PopOverlay(const Ref<Layer>& overlay)
	{
		VZ_PROFILE_FUNC()

		auto it = std::find(begin(), end(), overlay);
		if (it != end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}
}

