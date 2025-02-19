#include <Eucledia.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Eucledia::Layer
{
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0)
	{
		_triangleVA.reset(Eucledia::VertexArray::create());

		float vertices[3 * 7] = {
			-1.5, 0, 0, .5, 0, 1, 1,
			-.5, 0, 0, 1, 1, 1, 1,
			-1, .5, 0, 1, 0, 1, 1
		};

		std::shared_ptr<Eucledia::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Eucledia::VertexBuffer::create(vertices, sizeof(vertices)));
		vertexBuffer->setLayout({
			{ Eucledia::ShaderDataType::Float3, "position" },
			{ Eucledia::ShaderDataType::Float4, "color" }
		});
		_triangleVA->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Eucledia::IndexBuffer> indexBuffer;
		indexBuffer.reset(Eucledia::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		_triangleVA->setIndexBuffer(indexBuffer);

		_squareVA.reset(Eucledia::VertexArray::create());

		float squareVertices[5 * 4] = {
			-.5, -.5, 0, 0, 0,
			.5, -.5, 0, 1, 0,
			.5, .5, 0, 1, 1,
			-.5, .5, 0, 0, 1
		};

		std::shared_ptr<Eucledia::VertexBuffer> squareVB;
		squareVB.reset(Eucledia::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ Eucledia::ShaderDataType::Float3, "position" },
			{ Eucledia::ShaderDataType::Float2, "textCoord" },
		});
		_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Eucledia::IndexBuffer> squareIB;
		squareIB.reset(Eucledia::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->setIndexBuffer(squareIB);

		_shaderLibrary.load("assets/shaders/triangle.glsl");
		_shaderLibrary.load("assets/shaders/square.glsl");
		auto textureShader =  _shaderLibrary.load("assets/shaders/texture.glsl");
		_texture = Eucledia::Texture2D::create("assets/textures/default.png");

		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(textureShader)->uploadUniformInt("u_texture", 0);
	}

	void onUpdate(Eucledia::Timestep ts) override
	{
		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_A))
		{
			_cameraPosition.x -= _cameraMoveSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_D))
		{
			_cameraPosition.x += _cameraMoveSpeed * ts;
		}

		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_W))
		{
			_cameraPosition.y += _cameraMoveSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_S))
		{
			_cameraPosition.y -= _cameraMoveSpeed * ts;
		}

		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_Q))
		{
			_cameraRotation += _cameraRotationSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_E))
		{
			_cameraRotation -= _cameraRotationSpeed * ts;
		}

		Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
		Eucledia::RenderCommand::clear();

		_camera.setPosition(_cameraPosition);
		_camera.setRotation(_cameraRotation);

		Eucledia::Renderer::beginScene(_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.1));

		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_shaderLibrary.get("square"))->bind();
		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_shaderLibrary.get("square"))->uploadUniformFloat3("u_color", _squareColor);

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

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
		ImGui::End();
	}

	void onEvent(Eucledia::Event& event) override
	{

	}

private:
	Eucledia::ShaderLibrary _shaderLibrary;

	Eucledia::ref<Eucledia::VertexArray> _triangleVA, _squareVA;
	Eucledia::ref<Eucledia::Texture2D> _texture;

	Eucledia::OrthographicCamera _camera;
	glm::vec3 _cameraPosition;
	float _cameraMoveSpeed = 1.5f;

	float _cameraRotation = 0;
	float _cameraRotationSpeed = 180.f;

	glm::vec3 _squareColor = { 0.8, 0.8, 0.8 };
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