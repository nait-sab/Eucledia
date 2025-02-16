#include "euclediapch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Eucledia
{
	/// /////////////////////////////////////////////////
	/// VertexBuffer
	/// /////////////////////////////////////////////////

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGlVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	}

	void OpenGlVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/// /////////////////////////////////////////////////
	/// IndexBuffer
	/// /////////////////////////////////////////////////

	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGlIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	}

	void OpenGlIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}