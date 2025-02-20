#pragma once

#include "Eucledia/Core/Core.h"
#include "Eucledia/Core/Timestep.h"
#include "Eucledia/Events/Event.h"

namespace Eucledia
{
	class EUCLEDIA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}
