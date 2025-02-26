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

		void onUpdate(Timestep ts);
		void onViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry _registry;
		uint32_t _viewportWidth = 0, _viewportHeight = 0;

		friend class Entity;
	};
}
