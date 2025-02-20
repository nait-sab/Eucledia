#pragma once

#include "Eucledia/Core/Layer.h"

#include "Eucledia/Events/ApplicationEvent.h"
#include "Eucledia/Events/KeyEvent.h"
#include "Eucledia/Events/MouseEvent.h"

namespace Eucledia
{
	class EUCLEDIA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void begin();
		void end();

	private:
		float _time = 0;
	};
}