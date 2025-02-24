#include "euclediapch.h"
#include "Eucledia/Renderer/Framebuffer.h"

#include "Eucledia/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Eucledia
{
	ref<Framebuffer> Framebuffer::create(const FrameBufferSpecification& specification)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return createRef<OpenGLFramebuffer>(specification);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}