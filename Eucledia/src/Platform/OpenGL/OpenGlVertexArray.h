#pragma once

#include "Eucledia/Renderer/VertexArray.h"

namespace Eucledia
{
	class OpenGlVertexArray : public VertexArray
	{
	public:
		OpenGlVertexArray();
		virtual ~OpenGlVertexArray() ;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<ref<VertexBuffer>>& getVertexBuffers() const { return _vertexBuffers; }
		virtual const ref<IndexBuffer>& getIndexBuffer() const { return _indexBuffer; }

	private:
		uint32_t _rendererID;
		std::vector<ref<VertexBuffer>> _vertexBuffers;
		ref<IndexBuffer> _indexBuffer;
	};
}
