#include "euclediapch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Eucledia
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : _windowHandle(windowHandle)
	{
		EUCLEDIA_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EUCLEDIA_CORE_ASSERT(status, "Failed no initialize Glad");
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}
}