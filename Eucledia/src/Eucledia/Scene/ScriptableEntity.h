#pragma once

#include "Eucledia/Scene/Entity.h"

namespace Eucledia
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& getComponent()
		{
			return _entity.getComponent<T>();
		}

	private:
		Entity _entity;
		friend class Scene;
	};
}