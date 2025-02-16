#pragma once

#include "Eucledia/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Eucledia
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* _windowHandle;
	};
}

