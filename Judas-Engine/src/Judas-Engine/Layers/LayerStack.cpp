#include "jepch.h"
#include "LayerStack.h"

namespace Judas_Engine
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		JE_PROFILE_FUNC();

		for (auto layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		JE_PROFILE_FUNC()

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		layer->OnAttach();

		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		JE_PROFILE_FUNC()

		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		JE_PROFILE_FUNC()

		auto it = std::find(begin(), end(), layer);
		if (it != end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetach();
		}
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		JE_PROFILE_FUNC()

		auto it = std::find(begin(), end(), overlay);
		if (it != end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}
}

