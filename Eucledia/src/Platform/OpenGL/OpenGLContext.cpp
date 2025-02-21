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
		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EUCLEDIA_CORE_ASSERT(status, "Failed no initialize Glad");

		EUCLEDIA_CORE_INFO("OpenGL Info :");
		EUCLEDIA_CORE_INFO("	Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		EUCLEDIA_CORE_INFO("	Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		EUCLEDIA_CORE_INFO("	Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	#ifdef EUCLEDIA_ENABLE_ASSERTS
		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		EUCLEDIA_CORE_ASSERT(
			versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
			"Eucledia requires OpenGL version 4.5 or better"
		);
	#endif
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}
}