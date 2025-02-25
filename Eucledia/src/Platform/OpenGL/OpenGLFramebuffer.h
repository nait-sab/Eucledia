#pragma once

#include "Eucledia/Renderer/Framebuffer.h"

namespace Eucledia
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t getColorAttachmentRendererID() const override { return _colorAttachment; }

		virtual const FrameBufferSpecification& getSpecification() const override { return _specification; }

	private:
		uint32_t _rendererId = 0;
		uint32_t _colorAttachment = 0, _depthAttachment = 0;
		FrameBufferSpecification _specification;
	};
}

