#pragma once

#ifdef EUCLEDIA_PLATFORM_WNDOWS

extern Eucledia::Application* Eucledia::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Eucledia::CreateApplication();
	app->run();
	delete app;
}

#endif