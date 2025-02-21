#include "euclediapch.h"
#include "Eucledia/Renderer/RenderCommand.h"

namespace Eucledia
{
	scope<RendererAPI> RenderCommand::_rendererAPI = RendererAPI::create();
}