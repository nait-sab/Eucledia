#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(16.f / 9.f), _imguiColor({ 0.8, 0.2, 0.2, 1 })
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

	Eucledia::Renderer2D::resetStats();

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
		Eucledia::Renderer2D::drawQuad({ -1, 0 }, { .8, .8 }, _imguiColor);
		Eucledia::Renderer2D::drawQuad({ .5, -.5 }, { .5, .75 }, { 0.8, 0.2, 0.8, 1 });
		Eucledia::Renderer2D::drawQuad({ 0, 0, -0.1 }, { 20, 20 },  _texture, 10);
		Eucledia::Renderer2D::drawRotatedQuad({ -2, 0, 0 }, { 1, 1 }, rotation, _texture, 20);
		Eucledia::Renderer2D::endScene();

		Eucledia::Renderer2D::beginScene(_cameraController.getCamera());

		for (float y = -5.f; y < 5.f; y += .5)
		{
			for (float x = -5.f; x < 5.f; x += .5)
			{
				glm::vec4 color = { (x + 5.f) / 10.f, .4, (y + 5.f) / 10.f, 1 };
				Eucledia::Renderer2D::drawQuad({ x, y }, { .45, .45 }, color);
			}
		}

		Eucledia::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	EUCLEDIA_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Eucledia::Renderer2D::getStats();

    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quads: %d", stats.quadCount);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(_imguiColor));
    ImGui::Image(_texture->getRendererID(), ImVec2{ 256, 256 });
    ImGui::End();
}

void Sandbox2D::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
