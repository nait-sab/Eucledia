#pragma once

#ifdef EUCLEDIA_PLATFORM_WNDOWS

extern Eucledia::Application* Eucledia::CreateApplication();

int main(int argc, char** argv)
{
	Eucledia::Log::init();
	EUCLEDIA_CORE_INFO("Core init done");
	EUCLEDIA_INFO("Application start..");
	auto app = Eucledia::CreateApplication();
	app->run();
	delete app;
}

#endif