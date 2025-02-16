#pragma once

#include "Eucledia/Renderer/Buffer.h"

namespace Eucledia
{
	/// /////////////////////////////////////////////////
	/// VertexBuffer
	/// /////////////////////////////////////////////////

	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGlVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout& layout) override { _layout = layout; }
		virtual const BufferLayout& getlayout() const override { return _layout; }

	private:
		uint32_t _rendererID;
		BufferLayout _layout;
	};

	/// /////////////////////////////////////////////////
	/// IndexBuffer
	/// /////////////////////////////////////////////////

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const { return _count; }

	private:
		uint32_t _rendererID;
		uint32_t _count;
	};
}