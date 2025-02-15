#include "euclediapch.h"
#include "LayerStack.h"

namespace Eucledia
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layers)
		{
			delete layer;
		}
	}
	
	void LayerStack::pushLayer(Layer* layer)
	{
		_layers.emplace(_layers.begin() + _layerInsertIndex, layer);
		layer->onAttach();
		_layerInsertIndex++;
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		_layers.emplace_back(overlay);
		overlay->onAttach();
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(_layers.begin(), _layers.end(), layer);

		if (it != _layers.end())
		{
			_layers.erase(it);
			layer->onDetach();
			_layerInsertIndex--;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto it = std::find(_layers.begin(), _layers.end(), overlay);

		if (it != _layers.end())
		{
			_layers.erase(it);
			overlay->onDetach();
		}
	}
}