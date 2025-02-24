#include "ExampleLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer() : Layer("ExempleLayer"), _cameraController(16.f / 9.f)
{
	_triangleVA = Eucledia::VertexArray::create();

	float vertices[3 * 7] = {
		-1.5, 0, 0, .5, 0, 1, 1,
		-.5, 0, 0, 1, 1, 1, 1,
		-1, .5, 0, 1, 0, 1, 1
	};

	Eucledia::ref<Eucledia::VertexBuffer> vertexBuffer = Eucledia::VertexBuffer::create(vertices, sizeof(vertices));
	vertexBuffer->setLayout({
		{ Eucledia::ShaderDataType::Float3, "position" },
		{ Eucledia::ShaderDataType::Float4, "color" }
		});
	_triangleVA->addVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Eucledia::ref<Eucledia::IndexBuffer> indexBuffer = Eucledia::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
	_triangleVA->setIndexBuffer(indexBuffer);

	_squareVA = Eucledia::VertexArray::create();

	float squareVertices[5 * 4] = {
		-.5, -.5, 0, 0, 0,
		.5, -.5, 0, 1, 0,
		.5, .5, 0, 1, 1,
		-.5, .5, 0, 0, 1
	};

	Eucledia::ref<Eucledia::VertexBuffer> squareVB = Eucledia::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ Eucledia::ShaderDataType::Float3, "position" },
		{ Eucledia::ShaderDataType::Float2, "textCoord" },
		});
	_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Eucledia::ref<Eucledia::IndexBuffer> squareIB = Eucledia::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	_squareVA->setIndexBuffer(squareIB);

	_shaderLibrary.load("assets/shaders/triangle.glsl");
	_shaderLibrary.load("assets/shaders/square.glsl");
	auto textureShader = _shaderLibrary.load("assets/shaders/texture.glsl");
	_texture = Eucledia::Texture2D::create("assets/textures/default.png");

	textureShader->bind();
	textureShader->setInt("u_texture", 0);
}

void ExampleLayer::onAttach()
{
}

void ExampleLayer::onDetach()
{
}

void ExampleLayer::onUpdate(Eucledia::Timestep ts)
{
	_cameraController.onUpdate(ts);

	Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
	Eucledia::RenderCommand::clear();

	Eucledia::Renderer::beginScene(_cameraController.getCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.1f));

	_shaderLibrary.get("square")->bind();
	_shaderLibrary.get("square")->setFloat4("u_color", _squareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 position(x * .11f, y * .11f, 0);
			glm::mat4 transform = glm::translate(glm::mat4(1), position) * scale;
			Eucledia::Renderer::submit(_shaderLibrary.get("square"), _squareVA, transform);
		}
	}

	_texture->bind();
	Eucledia::Renderer::submit(_shaderLibrary.get("texture"), _squareVA, glm::scale(glm::mat4(1), glm::vec3(1.5)));
	Eucledia::Renderer::submit(_shaderLibrary.get("triangle"), _triangleVA);
	Eucledia::Renderer::endScene();
}

void ExampleLayer::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void ExampleLayer::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
