#include <Eucledia.h>

#include "imgui/imgui.h"

class ExampleLayer : public Eucledia::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void onUpdate() override
	{
		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_TAB))
		{
			EUCLEDIA_INFO("ExampleLayer::Update::Tab pressed");
		}
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Rabbit");
		ImGui::End();
	}

	void onEvent(Eucledia::Event& event) override
	{
		if (event.getEventType() == Eucledia::EventType::KeyPressed)
		{
			Eucledia::KeyPressedEvent& e = (Eucledia::KeyPressedEvent&)event;
			EUCLEDIA_TRACE("{0}", (char)e.getKeyCode());
		}
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