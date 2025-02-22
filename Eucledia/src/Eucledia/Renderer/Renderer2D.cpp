#include "euclediapch.h"
#include "Eucledia/Renderer/Renderer2D.h"

#include "Eucledia/Renderer/VertexArray.h"
#include "Eucledia/Renderer/Shader.h"
#include "Eucledia/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eucledia
{
	struct Renderer2DStore
	{
		ref<VertexArray> quadVA;
		ref<Shader> textureShader;
		ref<Texture2D> emptyTexture;
	};

	static Renderer2DStore* _store;

	void Renderer2D::init()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_store = new Renderer2DStore();
		_store->quadVA = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float2, "textCoord" }
		});
		_store->quadVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_store->quadVA->setIndexBuffer(squareIB);

		_store->emptyTexture = Texture2D::create(1, 1);
		uint32_t emptyTextureData = 0xffffffff;
		_store->emptyTexture->setData(&emptyTextureData, sizeof(uint32_t));

		_store->textureShader = Shader::create("assets/shaders/texture.glsl");
		_store->textureShader->bind();
		_store->textureShader->setInt("u_texture", 0);
	}

	void Renderer2D::shutdown()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		delete _store;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_store->textureShader->bind();
		_store->textureShader->setMat4("viewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
		EUCLEDIA_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		// Create the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);
		_store->textureShader->setFloat4("u_color", color);
		_store->textureShader->setFloat("u_multiplier", 1);
		_store->emptyTexture->bind();

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0 }, size, texture, multiplier);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		// Create the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);
		_store->textureShader->setFloat4("u_color", tintColor);
		_store->textureShader->setFloat("u_multiplier", multiplier);
		texture->bind();

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		// Create the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 });
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);
		_store->textureShader->setFloat4("u_color", color);
		_store->textureShader->setFloat("u_multiplier", 1);
		_store->emptyTexture->bind();

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture, multiplier, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier, const glm::vec4& tintColor)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		// Create the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 });
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);
		_store->textureShader->setFloat4("u_color", tintColor);
		_store->textureShader->setFloat("u_multiplier", multiplier);
		texture->bind();

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}
}