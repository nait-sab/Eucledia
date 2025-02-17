#include "euclediapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eucledia
{
	RendererAPI* RenderCommand::_rendererAPI = new OpenGLRendererAPI;
}