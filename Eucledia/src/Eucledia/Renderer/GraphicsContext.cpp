#include "euclediapch.h"
#include "Eucledia/Renderer/GraphicsContext.h"

#include "Eucledia/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Eucledia
{
	scope<GraphicsContext> GraphicsContext::create(void* window)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}