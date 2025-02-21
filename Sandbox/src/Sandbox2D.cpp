#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280 / 780)
{
}

void Sandbox2D::onAttach()
{
	
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Eucledia::Timestep ts)
{
	_cameraController.onUpdate(ts);

	Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
	Eucledia::RenderCommand::clear();

	Eucledia::Renderer2D::beginScene(_cameraController.getCamera());
	Eucledia::Renderer2D::drawQuad({0, 0}, {1, 1}, {0.8, 0.2, 0.2, 1});
	Eucledia::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
