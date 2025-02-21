#include "euclediapch.h"
#include "Eucledia/Renderer/Texture.h"

#include "Eucledia/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Eucledia
{
	ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGLTexture2D>(width, height);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGLTexture2D>(path);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}