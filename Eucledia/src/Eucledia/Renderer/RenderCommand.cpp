#include "euclediapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eucledia
{
	scope<RendererAPI> RenderCommand::_rendererAPI = createScope<OpenGLRendererAPI>();
}