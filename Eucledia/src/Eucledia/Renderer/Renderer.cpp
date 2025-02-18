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

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		shader->uploadUniformMat4("viewProjection", _sceneData->_viewProjectionMatrix);
		shader->uploadUniformMat4("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}