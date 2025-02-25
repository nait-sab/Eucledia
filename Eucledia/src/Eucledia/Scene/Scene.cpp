#include "euclediapch.h"
#include "Eucledia/Scene/Scene.h"

#include "Eucledia/Scene/Components.h"
#include "Eucledia/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Eucledia
{
	static void doMaths(const glm::mat4& transform)
	{

	}

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::createEntity()
	{
		return _registry.create();
	}

	void Scene::onUpdate(Timestep ts)
	{
		auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::drawQuad(transform, sprite);
		}
	}
}
