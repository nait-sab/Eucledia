#pragma once

#ifdef EUCLEDIA_PLATFORM_WNDOWS

extern Eucledia::Application* Eucledia::CreateApplication();

int main(int argc, char** argv)
{
	Eucledia::Log::init();
	EUCLEDIA_CORE_WARN("Initialized Log");
	EUCLEDIA_CORE_INFO("Hello");
	EUCLEDIA_CORE_ERROR("Test");
	EUCLEDIA_CORE_TRACE("Test");

	auto app = Eucledia::CreateApplication();
	app->run();
	delete app;
}

#endif