#pragma once

#include "Eucledia/Core/Core.h"
#include "Eucledia/Core/Layer.h"

#include <vector>

namespace Eucledia
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return _layers.begin(); }
		std::vector<Layer*>::iterator end() { return _layers.end(); }

	private:
		std::vector<Layer*> _layers;
		unsigned int _layerInsertIndex = 0;
	};
}

