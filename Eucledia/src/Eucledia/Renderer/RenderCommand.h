#pragma once

#include "RendererAPI.h"

namespace Eucledia
{
	class RenderCommand
	{
	public:
		inline static void init()
		{
			_rendererAPI->init();
		}

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			_rendererAPI->setViewport(x, y, width, height);
		}

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
		static scope<RendererAPI> _rendererAPI;
	};
}

