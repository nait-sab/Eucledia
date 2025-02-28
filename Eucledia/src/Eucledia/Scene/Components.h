#pragma once

#include <glm/glm.hpp>

#include <Eucledia/Scene/SceneCamera.h>
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
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		/* Functions to create and destroy the entity */
		std::function<void()> instantiateFunction;
		std::function<void()> destroyInstanceFunction;

		/* Native functions to manage the entity */
		std::function<void(ScriptableEntity*)> onCreateFunction;
		std::function<void(ScriptableEntity*, Timestep)> onUpdateFunction;
		std::function<void(ScriptableEntity*)> onDestroyFunction;

		template<typename T>
		void bind()
		{
			instantiateFunction = [&]() { instance = new T(); };
			destroyInstanceFunction = [&]() { delete (T*)instance; instance = nullptr; };

			onCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->onCreate(); };
			onUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->onUpdate(ts); };
			onDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->onDestroy(); };
		}
	};
}
