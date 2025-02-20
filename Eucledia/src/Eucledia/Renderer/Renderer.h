#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Eucledia
{
	class Renderer
	{
	public:
		static void init();
		static void onWindowRisized(uint32_t width, uint32_t height);

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const ref<Shader>& shader, const ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1));

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 _viewProjectionMatrix;
		};

		static SceneData* _sceneData;
	};
}