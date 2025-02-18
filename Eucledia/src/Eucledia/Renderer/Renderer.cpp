#include "euclediapch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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

	void Renderer::submit(const ref<Shader>& shader, const ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("viewProjection", _sceneData->_viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}