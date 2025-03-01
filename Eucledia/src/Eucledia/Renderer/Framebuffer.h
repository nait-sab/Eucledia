#pragma once

#include "Eucledia/Core/Base.h"

namespace Eucledia
{
	struct FrameBufferSpecification
	{
		uint32_t width = 0, height = 0;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& getSpecification() const = 0;

		static ref<Framebuffer> create(const FrameBufferSpecification& specification);
	};
}