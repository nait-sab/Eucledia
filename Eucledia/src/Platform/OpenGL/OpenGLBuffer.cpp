#include "euclediapch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Eucledia
{
	/// /////////////////////////////////////////////////
	/// VertexBuffer
	/// /////////////////////////////////////////////////

	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGlVertexBuffer::bind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	}

	void OpenGlVertexBuffer::unbind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::setData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/// /////////////////////////////////////////////////
	/// IndexBuffer
	/// /////////////////////////////////////////////////

	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGlIndexBuffer::bind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	}

	void OpenGlIndexBuffer::unbind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}