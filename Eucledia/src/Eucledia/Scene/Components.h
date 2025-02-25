#pragma once

#include <glm/glm.hpp>

#include <Eucledia/Renderer/Camera.h>

namespace Eucledia
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 transform{ 1 };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : transform(transform) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1, 1, 1, 1 };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}
	};

	struct CameraComponent
	{
		Camera camera;
		bool primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Camera& camera) : camera(camera) {}
	};
}
