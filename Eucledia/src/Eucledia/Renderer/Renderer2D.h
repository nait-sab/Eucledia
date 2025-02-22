#pragma once

#include "Eucledia/Renderer/OrthographicCamera.h"

#include "Eucledia/Renderer/Texture.h"

namespace Eucledia
{
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, float multiplier = 1, const glm::vec4& tintColor = glm::vec4(1));
	};
}
