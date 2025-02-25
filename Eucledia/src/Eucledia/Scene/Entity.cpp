#include "euclediapch.h"
#include "Eucledia/Scene/Entity.h"

namespace Eucledia
{
	Entity::Entity(entt::entity handle, Scene* scene) : _entityHandle(handle), _scene(scene)
	{
	}
}
