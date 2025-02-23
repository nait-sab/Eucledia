#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(16.f / 9.f)
{
}

void Sandbox2D::onAttach()
{
	EUCLEDIA_PROFILE_FUNCTION();

	_texture = Eucledia::Texture2D::create("assets/textures/default.png");
}

void Sandbox2D::onDetach()
{
	EUCLEDIA_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Eucledia::Timestep ts)
{
	EUCLEDIA_PROFILE_FUNCTION();

	_cameraController.onUpdate(ts);

	{
		EUCLEDIA_PROFILE_SCOPE("Renderer clean");
		Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
		Eucledia::RenderCommand::clear();
	}

	{
		EUCLEDIA_PROFILE_SCOPE("Renderer draw");

		static float rotation = 0.0f;
		rotation += ts * 40;

		Eucledia::Renderer2D::beginScene(_cameraController.getCamera());
		Eucledia::Renderer2D::drawRotatedQuad({ 1, 0 }, { .8, .8 }, -45, {0.5, 0.2, 0.6, 1});
		Eucledia::Renderer2D::drawQuad({ -1, 0 }, { .8, .8 }, { 0.8, 0.2, 0.2, 1 });
		Eucledia::Renderer2D::drawQuad({ .5, -.5 }, { .5, .75 }, { 0.8, 0.2, 0.8, 1 });
		Eucledia::Renderer2D::drawQuad({ 0, 0, -0.1 }, { 10, 10 },  _texture, 10);
		Eucledia::Renderer2D::drawRotatedQuad({ -2, 0, 0 }, { 1, 1 }, rotation, _texture, 20);
		Eucledia::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	EUCLEDIA_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
