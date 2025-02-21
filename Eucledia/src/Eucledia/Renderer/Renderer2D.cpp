#include "euclediapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eucledia
{
	struct Renderer2DStore
	{
		ref<VertexArray> quadVA;
		ref<Shader> baseColorShader;
	};

	static Renderer2DStore* _store;

	void Renderer2D::init()
	{
		_store = new Renderer2DStore();
		_store->quadVA = VertexArray::create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "position" }
		});
		_store->quadVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_store->quadVA->setIndexBuffer(squareIB);

		_store->baseColorShader = Shader::create("assets/shaders/square.glsl");
	}

	void Renderer2D::shutdown()
	{
		delete _store;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_store->baseColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(_store->baseColorShader)->uploadUniformMat4(
			"viewProjection", 
			camera.getViewProjectionMatrix()
		);
		std::dynamic_pointer_cast<OpenGLShader>(_store->baseColorShader)->uploadUniformMat4(
			"transform",
			glm::mat4(1)
		);
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
		std::dynamic_pointer_cast<OpenGLShader>(_store->baseColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(_store->baseColorShader)->uploadUniformFloat4("u_color", color);
		_store->quadVA->bind();
		RenderCommand::drawIndexed(_store->quadVA);
	}
}