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
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent) {
				if (!nativeScriptComponent.instance)
				{
					nativeScriptComponent.instantiateFunction();
					nativeScriptComponent.instance->_entity = Entity{ entity, this };
					nativeScriptComponent.onCreateFunction(nativeScriptComponent.instance);
				}

				nativeScriptComponent.onUpdateFunction(nativeScriptComponent.instance, ts);
			});
		}

		// Update the current active camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto view = _registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary)
			{
				mainCamera = &camera.camera;
				cameraTransform = &transform.transform;
				break;
			}
		}

		// Draw every entites with both transform and sprite component
		if (mainCamera)
		{
			Renderer2D::beginScene(*mainCamera, *cameraTransform);

			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform, sprite.color);
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
}
