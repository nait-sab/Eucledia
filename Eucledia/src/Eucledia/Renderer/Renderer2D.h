#pragma once

#include "Eucledia/Renderer/OrthographicCamera.h"

#include "Eucledia/Renderer/Texture.h"

#include "Eucledia/Renderer/Camera.h"
#include "Eucledia/Renderer/EditorCamera.h"

#include "Eucledia/Scene/Components.h"

namespace Eucledia
{
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const Camera& camera, const glm::mat4& transform);
		static void beginScene(const EditorCamera& camera);
		static void beginScene(const OrthographicCamera& camera);
		static void endScene();
		static void flush();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1), int entityID = -1);

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));

		static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityID);
	
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() const { return quadCount * 4; }
			uint32_t getTotalIndexCount() const { return quadCount * 6; }
		};

		static void resetStats();
		static Statistics getStats();

	private:
		static void startBatch();
		static void nextBatch();
	};
}
