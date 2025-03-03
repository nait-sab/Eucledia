#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Eucledia/Scene/SceneCamera.h"
#include "Eucledia/Scene/ScriptableEntity.h"

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
		glm::vec3 translation = { 0.f, 0.f, 0.f };
		glm::vec3 rotation = { 0.f, 0.f, 0.f };
		glm::vec3 scale = { 1.f, 1.f, 1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) {}

		glm::mat4 getTransform() const
		{
			glm::mat4 translationCalculated = glm::translate(glm::mat4(1.f), translation);

			glm::mat4 rotationCalculated = glm::rotate(glm::mat4(1.f), rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.f), rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.f), rotation.z, { 0, 0, 1 });

			glm::mat4 scaleCalculated = glm::scale(glm::mat4(1.f), scale);

			return translationCalculated * rotationCalculated * scaleCalculated;
		}
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
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiateScript)();
		void(*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* component) { delete component->instance; component->instance = nullptr; };
		}
	};
}
