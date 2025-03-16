#include "euclediapch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Eucledia
{
	static const uint32_t _maxFramebufferSize = 8192;

	namespace Utils
	{
		static GLenum textureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void createTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(textureTarget(multisampled), count, outID);
		}

		static void bindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(textureTarget(multisampled), id);
		}

		static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;

			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
		}

		static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;

			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
		}

		static bool isDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}

			return false;
		}

		static GLenum convertFBTextureFormatToGLenum(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			EUCLEDIA_CORE_ASSERT(false, "OpenGLFramebuffer::Utils::convertFBTextureFormatToGLenum Format not convertable")
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FrameBufferSpecification& specification) : _specification(specification)
	{
		for (auto spec : _specification.attachments.attachments)
		{
			if (!Utils::isDepthFormat(spec.textureFormat))
			{
				_colorAttachmentSpecifications.emplace_back(spec);
			}
			else
			{
				_depthAttachmentSpecification = spec;
			}
		}

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_rendererId);
		glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
		glDeleteTextures(1, &_depthAttachment);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (_rendererId)
		{
			glDeleteFramebuffers(1, &_rendererId);
			glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
			glDeleteTextures(1, &_depthAttachment);

			_colorAttachments.clear();
			_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &_rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

		bool multisample = _specification.samples > 1;

		// Attachments
		if (_colorAttachmentSpecifications.size())
		{
			_colorAttachments.resize(_colorAttachmentSpecifications.size());
			Utils::createTextures(multisample, _colorAttachments.data(), _colorAttachments.size());

			for (size_t index = 0; index < _colorAttachments.size(); index++)
			{
				Utils::bindTexture(multisample, _colorAttachments[index]);

				switch (_colorAttachmentSpecifications[index].textureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::attachColorTexture(
							_colorAttachments[index], _specification.samples, 
							GL_RGBA8, GL_RGBA,
							_specification.width, _specification.height, 
							index
						);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::attachColorTexture(
							_colorAttachments[index], _specification.samples,
							GL_R32I, GL_RED_INTEGER,
							_specification.width, _specification.height,
							index
						);
						break;
				}
			}
		}

		if (_depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::createTextures(multisample, &_depthAttachment, 1);
			Utils::bindTexture(multisample, _depthAttachment);

			switch (_depthAttachmentSpecification.textureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::attachDepthTexture(
						_depthAttachment, _specification.samples, 
						GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, 
						_specification.width, _specification.height
					);
					break;
			}
		}

		if (_colorAttachments.size() > 1)
		{
			EUCLEDIA_CORE_ASSERT(_colorAttachments.size() <= 4, "OpenGLFramebuffer::invalidate Color attachments count is wrong");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(_colorAttachments.size(), buffers);
		}
		else if (_colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}
		
		EUCLEDIA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);
		glViewport(0, 0, _specification.width, _specification.height);

		int value = -1;
		glClearTexImage(_colorAttachments[1], 0, GL_RED_INTEGER, GL_INT, &value);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > _maxFramebufferSize || height > _maxFramebufferSize)
		{
			EUCLEDIA_CORE_WARN("Trying to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		_specification.width = width;
		_specification.height = height;
		invalidate();
	}

	int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y)
	{
		EUCLEDIA_CORE_ASSERT(attachmentIndex < _colorAttachments.size(), "OpenGLFramebuffer::readPixel Attachment index not found");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		
		return pixelData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, int value)
	{
		EUCLEDIA_CORE_ASSERT(attachmentIndex < _colorAttachments.size(), "OpenGLFramebuffer::readPixel Attachment index not found");

		auto& spec = _colorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(
			_colorAttachments[attachmentIndex], 0, 
			Utils::convertFBTextureFormatToGLenum(spec.textureFormat), GL_INT,
			&value
		);
	}
}