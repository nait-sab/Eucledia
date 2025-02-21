#include "euclediapch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eucledia
{
	ref<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGlVertexArray>();
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}