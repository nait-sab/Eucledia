#pragma once

#include "Eucledia/Core/Core.h"

#ifdef EUCLEDIA_PLATFORM_WNDOWS

extern Eucledia::Application* Eucledia::CreateApplication();

int main(int argc, char** argv)
{
	Eucledia::Log::init();
	auto app = Eucledia::CreateApplication();
	app->run();
	delete app;
}

#endif