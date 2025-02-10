#include <Eucledia.h>

class Sandbox : public Eucledia::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Eucledia::Application* Eucledia::CreateApplication()
{
	return new Sandbox();
}