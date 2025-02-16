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

		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		float vertices[3 * 3] = {
			-.5f, -.5f, 0,
			.5f, -.5f, 0,
			0, .5f, 0
		};

		_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = { 0, 1, 2 };
		_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			out vec3 v_position;

			void main()
			{
				v_position = position;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main()
			{
				color = vec4(v_position * .5 + .5, 1.0);
			}
		)";

		_shader.reset(new Shader(vertexSrc, fragmentSrc));
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

			_shader->bind();
			glBindVertexArray(_vertexArray);
			glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
