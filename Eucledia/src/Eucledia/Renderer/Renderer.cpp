#include "euclediapch.h"
#include "Renderer.h"

namespace Eucledia
{
	Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		_sceneData->_viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->uploadUniformMat4("viewProjection", _sceneData->_viewProjectionMatrix);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}