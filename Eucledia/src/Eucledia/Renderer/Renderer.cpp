#include "euclediapch.h"
#include "Eucledia/Renderer/Renderer.h"
#include "Eucledia/Renderer/Renderer2D.h"

namespace Eucledia
{
	scope<Renderer::SceneData> Renderer::_sceneData = createScope<Renderer::SceneData>();

	void Renderer::init()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
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
		shader->setMat4("viewProjection", _sceneData->_viewProjectionMatrix);
		shader->setMat4("transform", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}