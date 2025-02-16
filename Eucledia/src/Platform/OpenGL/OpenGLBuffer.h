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

		virtual void bind() const;
		virtual void unbind() const;

	private:
		uint32_t _rendererID;
	};

	/// /////////////////////////////////////////////////
	/// IndexBuffer
	/// /////////////////////////////////////////////////

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();

		virtual void bind() const;
		virtual void unbind() const;

		virtual uint32_t getCount() const { return _count; }

	private:
		uint32_t _rendererID;
		uint32_t _count;
	};
}