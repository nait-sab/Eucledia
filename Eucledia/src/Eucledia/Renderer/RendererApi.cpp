#include "euclediapch.h"
#include "Eucledia/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eucledia
{
	RendererAPI::API RendererAPI::_api = RendererAPI::API::OpenGL;

	scope<RendererAPI> RendererAPI::create()
	{
		switch (_api)
		{
		case RendererAPI::API::None:
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return createScope<OpenGLRendererAPI>();
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}