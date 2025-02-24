#pragma once

#include "Eucledia/Core/Base.h"

namespace Eucledia
{
	struct FrameBufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual uint32_t getColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& getSpecification() const = 0;

		static ref<Framebuffer> create(const FrameBufferSpecification& specification);
	};
}