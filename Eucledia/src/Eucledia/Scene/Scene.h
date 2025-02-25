#pragma once

#include "entt.hpp"

#include "Eucledia/Core/Timestep.h"

namespace Eucledia
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity createEntity();

		// Temp
		entt::registry& getRegistry() { return _registry; }

		void onUpdate(Timestep ts);

	private:
		entt::registry _registry;
	};
}
