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

	private:
		entt::registry _registry;

		friend class Entity;
	};
}
