#include <Eucledia.h>
#include <Eucledia/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class App : public Eucledia::Application
{
public:
	App() : Application("Eucledia App")
	{
		// pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~App()
	{

	}
};

Eucledia::Application* Eucledia::createApplication()
{
	return new App();
}