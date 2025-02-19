#include "euclediapch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Eucledia
{
	ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}