#include "euclediapch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Eucledia/Events/ApplicationEvent.h"
#include "Eucledia/Events/MouseEvent.h"
#include "Eucledia/Events/KeyEvent.h"

#include "Eucledia/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Eucledia
{
	static uint8_t _GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EUCLEDIA_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_data._title = props._title;
		_data._width = props._width;
		_data._height = props._height;

		EUCLEDIA_CORE_INFO("Creating window {0} ({1}, {2})", props._title, props._width, props._height);

		if (_GLFWWindowCount == 0)
		{
			EUCLEDIA_PROFILE_SCOPE("WindowsWindow::init glfw init");
			EUCLEDIA_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			EUCLEDIA_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			EUCLEDIA_PROFILE_SCOPE("WindowsWindow::init glfw create window");

		#if defined(EUCLEDIA_DEBUG)
			if (Renderer::getAPI() == RendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
		#endif

			_window = glfwCreateWindow((int)props._width, (int)props._height, _data._title.c_str(), nullptr, nullptr);
			_GLFWWindowCount++;
		}

		_context = OpenGLContext::create(_window);
		_context->init();
		glfwSetWindowUserPointer(_window, &_data);
		setVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data._width = width;
			data._height = height;
			WindowResizeEvent event(width, height);
			data._eventCallback(event);
		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data._eventCallback(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int modes)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data._eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data._eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data._eventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keyCode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keyCode);
			data._eventCallback(event);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data._eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data._eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data._eventCallback(event);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPosition, double yPosition)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPosition, (float)yPosition);
			data._eventCallback(event);
		});
	}

	void WindowsWindow::shutdown()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glfwDestroyWindow(_window);
		--_GLFWWindowCount;

		if (--_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glfwPollEvents();
		_context->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		_data._vSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return _data._vSync;
	}
}