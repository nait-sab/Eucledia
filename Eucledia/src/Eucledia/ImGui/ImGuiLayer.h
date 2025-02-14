#pragma once

#include "Eucledia/Layer.h"

namespace Eucledia
{
	class EUCLEDIA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

	private:
		float _time = 0;
	};
}