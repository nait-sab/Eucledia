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
		Eucledia::Renderer2D::beginScene(_cameraController.getCamera());
		Eucledia::Renderer2D::drawRotatedQuad({ -1, 0 }, { .8, .8 }, -glm::radians(24.5f), {0.8, 0.2, 0.2, 1});
		Eucledia::Renderer2D::drawQuad({ .5, -.5 }, { .5, .75 }, { 0.8, 0.2, 0.8, 1 });
		Eucledia::Renderer2D::drawRotatedQuad({ .5, .5 }, { .75, .7 }, glm::radians(24.5f), _texture, 3, { .8, .3, .3, 1 });
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
