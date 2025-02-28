#include "euclediapch.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
		EUCLEDIA_PROFILE_FUNCTION();

		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EUCLEDIA_CORE_ASSERT(status, "Failed no initialize Glad");

		EUCLEDIA_CORE_INFO("OpenGL Info :");
		EUCLEDIA_CORE_INFO("	Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		EUCLEDIA_CORE_INFO("	Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		EUCLEDIA_CORE_INFO("	Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		EUCLEDIA_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Eucledia requires OpenGL version 4.5 or better");
	}

	void OpenGLContext::swapBuffers()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glfwSwapBuffers(_windowHandle);
	}
}