#pragma once

#include "Eucledia/Core/Timestep.h"

#include <entt.hpp>

namespace Eucledia
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onUpdate(Timestep ts);
		void onViewportResize(uint32_t width, uint32_t height);

		Entity getPrimaryCameraEntity();

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;

		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
