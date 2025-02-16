#include "euclediapch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eucledia
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getRendererAPI())
		{
		case RendererAPI::None:
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGlVertexArray();
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}