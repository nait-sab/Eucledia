#include "euclediapch.h"
#include "Eucledia/Scene/Scene.h"

#include "Eucledia/Scene/Components.h"
#include "Eucledia/Scene/Entity.h"
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

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { _registry.create(), this };
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
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
