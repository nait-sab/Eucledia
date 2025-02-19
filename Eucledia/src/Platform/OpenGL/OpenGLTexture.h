#pragma once

#include "Eucledia/Renderer/Texture.h"

namespace Eucledia
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return _width; };
		virtual uint32_t getHeight() const override { return _height; };

		virtual void bind(uint32_t slot = 0) const override;

	private:
		std::string _path;
		uint32_t _width, _height;
		uint32_t _rendererID;
	};
}
