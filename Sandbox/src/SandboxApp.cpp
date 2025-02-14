#include <Eucledia.h>

class ExampleLayer : public Eucledia::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void onUpdate() override
	{
		//EUCLEDIA_INFO("ExampleLayer::Update");
	}

	void onEvent(Eucledia::Event& event) override
	{
		//EUCLEDIA_TRACE(event.toString());
	}
};

class Sandbox : public Eucledia::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Eucledia::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Eucledia::Application* Eucledia::CreateApplication()
{
	return new Sandbox();
}