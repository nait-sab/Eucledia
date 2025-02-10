#pragma once

#include "Core.h"

namespace Eucledia
{
	class EUCLEDIA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in client
	Application* CreateApplication();
}
