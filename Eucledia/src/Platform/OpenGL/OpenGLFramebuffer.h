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

		virtual uint32_t getColorAttachmentRendererID() const override { return _colorAttachment; }

		virtual const FrameBufferSpecification& getSpecification() const override { return _specification; }

	private:
		uint32_t _rendererId;
		uint32_t _colorAttachment, _depthAttachment;
		FrameBufferSpecification _specification;
	};
}

