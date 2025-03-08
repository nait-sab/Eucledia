#include "euclediapch.h"
#include "Eucledia/Scene/Scene.h"

#include "Eucledia/Scene/Components.h"
#include "Eucledia/Scene/Entity.h"
#include "Eucledia/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Eucledia
{
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

	void Scene::destroyEntity(Entity entity)
	{
		_registry.destroy(entity);
	}

	void Scene::onUpdate(Timestep ts)
	{
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent)
			{
				if (!nativeScriptComponent.instance)
				{
					nativeScriptComponent.instance = nativeScriptComponent.instantiateScript();
					nativeScriptComponent.instance->_entity = Entity{ entity, this };
					nativeScriptComponent.instance->onCreate();
				}

				nativeScriptComponent.instance->onUpdate(ts);
			});
		}

		// Update the current active camera
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = _registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary)
			{
				mainCamera = &camera.camera;
				cameraTransform = transform.getTransform();
				break;
			}
		}

		// Draw every entites with both transform and sprite component
		if (mainCamera)
		{
			Renderer2D::beginScene(*mainCamera, cameraTransform);

			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform.getTransform(), sprite.color);
			}

			Renderer2D::endScene();
		}
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		auto view = _registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponant = view.get<CameraComponent>(entity);

			if (!cameraComponant.fixedAspectRatio)
			{
				cameraComponant.camera.setViewportSize(width, height);
			}
		}
	}

	Entity Scene::getPrimaryCameraEntity()
	{
		auto view = _registry.view<CameraComponent>();

		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);

			if (camera.primary)
			{
				return Entity{ entity, this };
			}
		}

		return {};
	}

	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.setViewportSize(_viewportWidth, _viewportHeight);
	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}
