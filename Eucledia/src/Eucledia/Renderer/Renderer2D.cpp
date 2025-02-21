#include "euclediapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

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
		_store = new Renderer2DStore();
		_store->quadVA = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float2, "textCoord" }
		});
		_store->quadVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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
		delete _store;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		_store->textureShader->bind();
		_store->textureShader->setMat4("viewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		_store->textureShader->setFloat4("u_color", color);
		_store->emptyTexture->bind();

		// Setup the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0 }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture)
	{
		_store->textureShader->setFloat4("u_color", glm::vec4(1));
		texture->bind();

		// Setup the transform with the position and size
		glm::mat4 transform = glm::translate(glm::mat4(1), position);
		transform *= glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		_store->textureShader->setMat4("transform", transform);

		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}
}