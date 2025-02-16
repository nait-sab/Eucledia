#include "euclediapch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Eucledia
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) 

	Application* Application::_instance = nullptr;

	Application::Application()
	{
		EUCLEDIA_CORE_ASSERT(!_instance, "Application already exists")
		_instance = this;

		_window = std::unique_ptr<Window>(Window::create());
		_window->setEventCallback(BIND_EVENT_FN(onEvent));

		_imGuiLayer = new ImGuiLayer();
		pushOverlay(_imGuiLayer);

		_vertexArray.reset(VertexArray::create());

		float vertices[3 * 7] = {
			-.5, -.5, 0, .5, 0, 1, 1,
			.5, -.5, 0, 1, 1, 1, 1,
			0, .5, 0, 1, 0, 1, 1
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		vertexBuffer->setLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float4, "color" }
		});
		_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		_vertexArray->setIndexBuffer(indexBuffer);

		_squareVA.reset(VertexArray::create());

		float squareVertices[3 * 4] = {
			-.75, -.75, 0,
			.75, -.75, 0,
			.75, .75, 0,
			-.75, .75, 0
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "position" },
		});
		_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = position;
				v_color = color;
				gl_Position = vec4(position, 1.0);
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

		_shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			out vec3 v_position;

			void main()
			{
				v_position = position;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main()
			{
				color = vec4(0.7, 0, 0.6, 1.0);
			}
		)";

		_squareShader.reset(new Shader(vertexSrc2, fragmentSrc2));
	}

	Application::~Application()
	{
		delete _imGuiLayer;
	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		_layerStack.pushOverlay(layer);
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClosed));

		for (auto it = _layerStack.end(); it != _layerStack.begin();)
		{
			(*--it)->onEvent(event);
			
			if (event._handled)
			{
				break;
			}
		}
	}

	void Application::run()
	{
		while (_running)
		{
			glClearColor(.15f, .15f, .15f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_squareShader->bind();
			_squareVA->bind();
			glDrawElements(GL_TRIANGLES, _squareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

			_shader->bind();
			_vertexArray->bind();
			glDrawElements(GL_TRIANGLES, _vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : _layerStack)
			{
				layer->onUpdate();
			}

			_imGuiLayer->begin();

			for (Layer* layer : _layerStack)
			{
				layer->onImGuiRender();
			}

			_imGuiLayer->end();

			_window->onUpdate();
		}
	}

	bool Application::onWindowClosed(WindowCloseEvent& event)
	{
		_running = false;
		return true;
	}
}
