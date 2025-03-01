#pragma once

#include "Eucledia/Scene/Entity.h"

namespace Eucledia
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& getComponent()
		{
			return _entity.getComponent<T>();
		}

	protected:
		virtual void onCreate() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onDestroy() {}

	private:
		Entity _entity;
		friend class Scene;
	};
}