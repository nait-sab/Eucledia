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
			-.5, -.5, 0, .5, 0, 1, 1,
			.5, -.5, 0, 1, 1, 1, 1,
			0, .5, 0, 1, 0, 1, 1
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

		float squareVertices[3 * 4] = {
			-.5, -.5, 0,
			.5, -.5, 0,
			.5, .5, 0,
			-.5, .5, 0
		};

		std::shared_ptr<Eucledia::VertexBuffer> squareVB;
		squareVB.reset(Eucledia::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ Eucledia::ShaderDataType::Float3, "position" },
		});
		_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Eucledia::IndexBuffer> squareIB;
		squareIB.reset(Eucledia::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = position;
				v_color = color;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position * .5 + .5, 1.0);
				color = v_color;
			}
		)";

		_triangleShader.reset(Eucledia::Shader::create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec3 v_position;

			void main()
			{
				v_position = position;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		_flatColorShader.reset(Eucledia::Shader::create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	void onUpdate(Eucledia::Timestep ts) override
	{
		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_A))
		{
			_cameraPosition.x += _cameraMoveSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_D))
		{
			_cameraPosition.x -= _cameraMoveSpeed * ts;
		}

		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_W))
		{
			_cameraPosition.y -= _cameraMoveSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_S))
		{
			_cameraPosition.y += _cameraMoveSpeed * ts;
		}

		if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_Q))
		{
			_cameraRotation -= _cameraRotationSpeed * ts;
		}
		else if (Eucledia::Input::isKeyPressed(EUCLEDIA_KEY_E))
		{
			_cameraRotation += _cameraRotationSpeed * ts;
		}

		Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
		Eucledia::RenderCommand::clear();

		_camera.setPosition(_cameraPosition);
		_camera.setRotation(_cameraRotation);

		Eucledia::Renderer::beginScene(_camera);

		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_flatColorShader)->bind();
		std::dynamic_pointer_cast<Eucledia::OpenGLShader>(_flatColorShader)->uploadUniformFloat3("u_color", _squareColor);

		Eucledia::Renderer::submit(_flatColorShader, _squareVA);


		Eucledia::Renderer::submit(_triangleShader, _triangleVA);
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
	Eucledia::ref<Eucledia::Shader> _triangleShader;
	Eucledia::ref<Eucledia::VertexArray> _triangleVA;

	Eucledia::ref<Eucledia::Shader> _flatColorShader;
	Eucledia::ref<Eucledia::VertexArray> _squareVA;

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