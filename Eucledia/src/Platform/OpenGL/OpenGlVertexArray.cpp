#include "euclediapch.h"
#include "OpenGlVertexArray.h"

#include <glad/glad.h>

namespace Eucledia
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Eucledia::ShaderDataType::Float:		return GL_FLOAT;
		case Eucledia::ShaderDataType::Float2:	return GL_FLOAT;
		case Eucledia::ShaderDataType::Float3:	return GL_FLOAT;
		case Eucledia::ShaderDataType::Float4:	return GL_FLOAT;
		case Eucledia::ShaderDataType::Mat3:	return GL_FLOAT;
		case Eucledia::ShaderDataType::Mat4:	return GL_FLOAT;
		case Eucledia::ShaderDataType::Int:		return GL_INT;
		case Eucledia::ShaderDataType::Int2:	return GL_INT;
		case Eucledia::ShaderDataType::Int3:	return GL_INT;
		case Eucledia::ShaderDataType::Int4:	return GL_INT;
		case Eucledia::ShaderDataType::Bool:	return GL_BOOL;
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow ShaderDataType");
		return 0;
	}

	OpenGlVertexArray::OpenGlVertexArray()
	{
		glCreateVertexArrays(1, &_rendererID);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		glDeleteVertexArrays(1, &_rendererID);
	}

	void OpenGlVertexArray::bind() const
	{
		glBindVertexArray(_rendererID);
	}

	void OpenGlVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGlVertexArray::addVertexBuffer(const ref<VertexBuffer>& vertexBuffer)
	{
		EUCLEDIA_CORE_ASSERT(vertexBuffer->getlayout().getElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(_rendererID);
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getlayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(_vertexBufferIndex);
			glVertexAttribPointer(
				_vertexBufferIndex,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element._type),
				element._normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)(intptr_t)element._offset
			);
			_vertexBufferIndex++;
		}

		_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGlVertexArray::setIndexBuffer(const ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_rendererID);
		indexBuffer->bind();
		_indexBuffer = indexBuffer;
	}
}
