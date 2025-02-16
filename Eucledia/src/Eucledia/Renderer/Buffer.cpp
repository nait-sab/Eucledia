#include "euclediapch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Eucledia
{
	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::getRendererAPI())
		{
		case RendererAPI::None: 
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGlVertexBuffer(vertices, size);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::getRendererAPI())
		{
		case RendererAPI::None:
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGlIndexBuffer(indices, size);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}