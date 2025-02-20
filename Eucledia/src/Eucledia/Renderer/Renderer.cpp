#include "euclediapch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eucledia
{
	scope<Renderer::SceneData> Renderer::_sceneData = createScope<Renderer::SceneData>();

	void Renderer::init()
	{
		RenderCommand::init();
	}

	void Renderer::onWindowRisized(uint32_t width, uint32_t height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		_sceneData->_viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const ref<Shader>& shader, const ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("viewProjection", _sceneData->_viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}