#include "euclediapch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Eucledia
{
	ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None: 
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGlVertexBuffer>(vertices, size);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGlIndexBuffer>(indices, size);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}