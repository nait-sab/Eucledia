#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(16 / 9)
{
}

void Sandbox2D::onAttach()
{
	_squareVA = Eucledia::VertexArray::create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	std::shared_ptr<Eucledia::VertexBuffer> squareVB;
	squareVB = Eucledia::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ Eucledia::ShaderDataType::Float3, "position" },
		});
	_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<Eucledia::IndexBuffer> squareIB;
	squareIB = Eucledia::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	_squareVA->setIndexBuffer(squareIB);

	_shaderLibrary.load("assets/shaders/square.glsl");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Eucledia::Timestep ts)
{
	_cameraController.onUpdate(ts);

	Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
	Eucledia::RenderCommand::clear();

	Eucledia::Renderer::beginScene(_cameraController.getCamera());

	std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_shaderLibrary.get("square"))->bind();
	std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_shaderLibrary.get("square"))->uploadUniformFloat4("u_color", _squareColor);

	Eucledia::Renderer::submit(_shaderLibrary.get("square"), _squareVA, glm::scale(glm::mat4(1), glm::vec3(1.5)));
	Eucledia::Renderer::endScene();
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
