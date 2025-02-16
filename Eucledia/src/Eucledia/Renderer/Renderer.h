#pragma once

namespace Eucledia
{
	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI getRendererAPI() { return _rendererAPI; }

	private:
		static RendererAPI _rendererAPI;
	};
}