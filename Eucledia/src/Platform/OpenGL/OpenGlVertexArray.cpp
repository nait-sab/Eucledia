#include "euclediapch.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"

#include <glad/glad.h>

namespace Eucledia
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow ShaderDataType");
		return 0;
	}

	OpenGlVertexArray::OpenGlVertexArray()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &_rendererID);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &_rendererID);
	}

	void OpenGlVertexArray::bind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindVertexArray(_rendererID);
	}

	void OpenGlVertexArray::unbind() const
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGlVertexArray::addVertexBuffer(const ref<VertexBuffer>& vertexBuffer)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		EUCLEDIA_CORE_ASSERT(vertexBuffer->getlayout().getElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(_rendererID);
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getlayout();
		for (const auto& element : layout)
		{
			switch (element._type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(_vertexBufferIndex);
					glVertexAttribPointer(
						_vertexBufferIndex,
						element.getComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element._type),
						element._normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)element._offset
					);
					_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.getComponentCount();
					
					for (uint8_t index = 0; index < count; index++)
					{
						glEnableVertexAttribArray(_vertexBufferIndex);
						glVertexAttribPointer(
							_vertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element._type),
							element._normalized ? GL_TRUE : GL_FALSE,
							layout.getStride(),
							(const void*)(sizeof(float) * count * 1)
						);
						glVertexAttribDivisor(_vertexBufferIndex, 1);
						_vertexBufferIndex++;
					}
					break;
				}
				default:
					EUCLEDIA_CORE_ASSERT(false, "Unknow ShaderDataType");
			}
		}

		_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGlVertexArray::setIndexBuffer(const ref<IndexBuffer>& indexBuffer)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glBindVertexArray(_rendererID);
		indexBuffer->bind();
		_indexBuffer = indexBuffer;
	}
}
