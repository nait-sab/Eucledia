#pragma once

#include "Eucledia/Renderer/RendererAPI.h"

namespace Eucledia
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndexed(const ref<VertexArray>& vertexArray) override;
	};
}

