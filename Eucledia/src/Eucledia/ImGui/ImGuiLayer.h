#pragma once

#include "Eucledia/Core/Layer.h"

#include "Eucledia/Events/ApplicationEvent.h"
#include "Eucledia/Events/KeyEvent.h"
#include "Eucledia/Events/MouseEvent.h"

namespace Eucledia
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& event) override;

		void begin();
		void end();

		void blockEvents(bool block) { _blockEvents = block; }
		void setDarkThemeColors();

	private:
		bool _blockEvents = true;
	};
}