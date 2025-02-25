#pragma once

#include "Eucledia/Core/Base.h"

#ifdef EUCLEDIA_PLATFORM_WNDOWS

extern Eucledia::Application* Eucledia::createApplication();

int main(int argc, char** argv)
{
	Eucledia::Log::init();

	EUCLEDIA_PROFILE_BEGIN_SESSION("Startup", "EuclediaProfile-Startup.json");
	auto app = Eucledia::createApplication();
	EUCLEDIA_PROFILE_END_SESSION();

	EUCLEDIA_PROFILE_BEGIN_SESSION("Runtime", "EuclediaProfile-Runtime.json");
	app->run();
	EUCLEDIA_PROFILE_END_SESSION();

	EUCLEDIA_PROFILE_BEGIN_SESSION("Shutdown", "EuclediaProfile-Shutdown.json");
	delete app;
	EUCLEDIA_PROFILE_END_SESSION();
}

#endif