#pragma once

#include "RendererAPI.h"

namespace Eucledia
{
	class RenderCommand
	{
	public:
		inline static void setClearColor(const glm::vec4& color)
		{
			_rendererAPI->setClearColor(color);
		}

		inline static void clear()
		{
			_rendererAPI->clear();
		}

		inline static void drawIndexed(const ref<VertexArray>& vertexArray)
		{
			_rendererAPI->drawIndexed(vertexArray);
		}

	private:
		static RendererAPI* _rendererAPI;
	};
}

