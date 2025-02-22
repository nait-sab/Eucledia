#include <Eucledia.h>
#include <Eucledia/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Eucledia::Application
{
public:
	Sandbox()
	{
		// pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Eucledia::Application* Eucledia::CreateApplication()
{
	return new Sandbox();
}