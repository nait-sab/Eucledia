#include <Eucledia.h>

class ExampleLayer : public Eucledia::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void onUpdate() override
	{
		// EUCLEDIA_INFO("ExampleLayer::Update");
	}
};

class Sandbox : public Eucledia::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Eucledia::Application* Eucledia::CreateApplication()
{
	return new Sandbox();
}