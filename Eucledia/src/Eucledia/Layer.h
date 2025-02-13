#pragma once

#include "Eucledia/Core.h"
#include "Eucledia/Events/Event.h"

namespace Eucledia
{
	class EUCLEDIA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}
