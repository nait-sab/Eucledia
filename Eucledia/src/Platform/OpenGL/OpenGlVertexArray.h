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

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return _vertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return _indexBuffer; }

	private:
		uint32_t _rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
		std::shared_ptr<IndexBuffer> _indexBuffer;
	};
}
