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
		virtual int readPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void clearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override 
		{ 
			EUCLEDIA_CORE_ASSERT(index < _colorAttachments.size(), "OpenGLFramebuffer::getColorAttachmentRendererID Index not found");
			return _colorAttachments[index]; 
		}

		virtual const FrameBufferSpecification& getSpecification() const override { return _specification; }

	private:
		uint32_t _rendererId = 0;
		FrameBufferSpecification _specification;

		std::vector<FramebufferTextureSpecification> _colorAttachmentSpecifications;
		FramebufferTextureSpecification _depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> _colorAttachments;
		uint32_t _depthAttachment = 0;
	};
}

