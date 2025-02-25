#include "euclediapch.h"
#include "Eucledia/Renderer/Renderer2D.h"

#include "Eucledia/Renderer/VertexArray.h"
#include "Eucledia/Renderer/Shader.h"
#include "Eucledia/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eucledia
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textCoord;
		float textureIndex;
		float textureMultiplier;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		ref<VertexArray> quadVA;
		ref<VertexBuffer> quadVB;
		ref<Shader> textureShader;
		ref<Texture2D> emptyTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVBBase = nullptr;
		QuadVertex* quadVBPointer = nullptr;

		std::array<ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotsIndex = 1; // 0 is for the empty texture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData _data;

	void Renderer2D::init()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_data.quadVA = VertexArray::create();

		_data.quadVB = VertexBuffer::create(_data.maxVertices * sizeof(QuadVertex));
		_data.quadVB->setLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float4, "color" },
			{ ShaderDataType::Float2, "textCoord" },
			{ ShaderDataType::Float, "textureIndex" },
			{ ShaderDataType::Float, "textureMultiplier" }
		});
		_data.quadVA->addVertexBuffer(_data.quadVB);
		_data.quadVBBase = new QuadVertex[_data.maxVertices];

		uint32_t* quadIndices = new uint32_t[_data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t index = 0; index < _data.maxIndices; index += 6)
		{
			quadIndices[index + 0] = offset + 0;
			quadIndices[index + 1] = offset + 1;
			quadIndices[index + 2] = offset + 2;

			quadIndices[index + 3] = offset + 2;
			quadIndices[index + 4] = offset + 3;
			quadIndices[index + 5] = offset + 0;

			offset += 4;
		}

		ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, _data.maxIndices);
		_data.quadVA->setIndexBuffer(quadIB);
		delete[] quadIndices;

		_data.emptyTexture = Texture2D::create(1, 1);
		uint32_t emptyTextureData = 0xffffffff;
		_data.emptyTexture->setData(&emptyTextureData, sizeof(uint32_t));

		int32_t samplers[_data.maxTextureSlots];

		for (uint32_t index = 0; index < _data.maxTextureSlots; index++)
		{
			samplers[index] = index;
		}

		_data.textureShader = Shader::create("assets/shaders/texture.glsl");
		_data.textureShader->bind();
		_data.textureShader->setIntArray("u_textures", samplers, _data.maxTextureSlots);

		// Set empty texture to first index
		_data.textureSlots[0] = _data.emptyTexture;

		_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void Renderer2D::shutdown()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		delete[] _data.quadVBBase;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_data.textureShader->bind();
		_data.textureShader->setMat4("viewProjection", camera.getViewProjectionMatrix());

		_data.quadIndexCount = 0;
		_data.quadVBPointer = _data.quadVBBase;
		_data.textureSlotsIndex = 1;
	}

	void Renderer2D::endScene()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)_data.quadVBPointer - (uint8_t*)_data.quadVBBase);
		_data.quadVB->setData(_data.quadVBBase, dataSize);

		flush();
	}

	void Renderer2D::flush()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		// Nothing to draw
		if (_data.quadIndexCount == 0)
		{
			return;
		}

		for (uint32_t index = 0; index < _data.textureSlotsIndex; index++)
		{
			_data.textureSlots[index]->bind(index);
		}

		RenderCommand::drawIndexed(_data.quadVA, _data.quadIndexCount);
		_data.stats.drawCalls++;
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		_data.quadIndexCount = 0;
		_data.quadVBPointer = _data.quadVBBase;
		_data.textureSlotsIndex = 1;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0 }, size, texture, multiplier);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		drawQuad(transform, texture, multiplier, tintColor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		// 0 is the empty texture
		const float textureIndex = 0;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float textureMultiplier = 1;

		if (_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		for (size_t index = 0; index < quadVertexCount; index++)
		{
			_data.quadVBPointer->position = transform * _data.quadVertexPositions[index];
			_data.quadVBPointer->color = color;
			_data.quadVBPointer->textCoord = textureCoords[index];
			_data.quadVBPointer->textureIndex = textureIndex;
			_data.quadVBPointer->textureMultiplier = textureMultiplier;
			_data.quadVBPointer++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (_data.quadIndexCount >= Renderer2DData::maxIndices)
		{
			flushAndReset();
		}

		float textureIndex = 0;

		// Skip index 0 of the empty texture
		for (uint32_t index = 1; index < _data.textureSlotsIndex; index++)
		{
			if (*_data.textureSlots[index].get() == *texture.get())
			{
				textureIndex = (float)index;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (_data.textureSlotsIndex >= Renderer2DData::maxTextureSlots)
			{
				flushAndReset();
			}

			textureIndex = (float)_data.textureSlotsIndex;
			_data.textureSlots[_data.textureSlotsIndex] = texture;
			_data.textureSlotsIndex++;
		}

		for (size_t index = 0; index < quadVertexCount; index++)
		{
			_data.quadVBPointer->position = transform * _data.quadVertexPositions[index];
			_data.quadVBPointer->color = tintColor;
			_data.quadVBPointer->textCoord = textureCoords[index];
			_data.quadVBPointer->textureIndex = textureIndex;
			_data.quadVBPointer->textureMultiplier = multiplier;
			_data.quadVBPointer++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::rotate(glm::mat4(1), glm::radians(rotation), { 0, 0, 1 });
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		drawQuad(transform, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture, multiplier, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::rotate(glm::mat4(1), glm::radians(rotation), { 0, 0, 1 });
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		drawQuad(transform, texture, multiplier, tintColor);
	}

	void Renderer2D::resetStats()
	{
		memset(&_data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return _data.stats;
	}
}