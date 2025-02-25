#pragma once

#include <glm/glm.hpp>

#include "Eucledia/Renderer/VertexArray.h"

namespace Eucledia
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		virtual ~RendererAPI() = default;

		virtual void init() = 0;
		static scope<RendererAPI> create();

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static API getAPI() { return _api; }

	private:
		static API _api;
	};
}