#pragma once

#include "Eucledia/Scene/Scene.h"

#include <entt.hpp>

namespace Eucledia
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			EUCLEDIA_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component");
			return _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			EUCLEDIA_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component");
			return _scene->_registry.get<T>(_entityHandle);
		}

		template<typename T>
		bool hasComponent()
		{
			return _scene->_registry.all_of<T>(_entityHandle);
		}

		template<typename T>
		void removeComponent()
		{
			EUCLEDIA_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component");
			_scene->_registry.remove<T>(_entityHandle);
		}

		operator bool() const { return _entityHandle != entt::null; }

	private:
		entt::entity _entityHandle{ entt::null };
		Scene* _scene = nullptr;
	};
}