#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Eucledia/Scene/Components.h"

namespace Eucledia
{
	SceneHierarchyPanel::SceneHierarchyPanel(const ref<Scene>& context)
	{
		setContext(context);
	}

	void SceneHierarchyPanel::setContext(const ref<Scene>& context)
	{
		_context = context;
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		_context->_registry.view<entt::entity>().each([&](auto entityID)
		{
			Entity entity{ entityID, _context.get() };
			drawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			_selectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (_selectionContext)
		{
			drawComponents(_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			_selectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<TransformComponent>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);
				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.getComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];

				ImGui::Checkbox("Primary", &cameraComponent.primary);

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int index = 0; index < 2; index++)
					{
						bool isSeleted = currentProjectionTypeString == projectionTypeStrings[index];

						if (ImGui::Selectable(projectionTypeStrings[index], isSeleted))
						{
							currentProjectionTypeString = projectionTypeStrings[index];
							camera.setProjectionType((SceneCamera::ProjectionType)index);
						}

						if (isSeleted)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveFOV = glm::degrees(camera.getPerspectiveFOV());
					float perspectiveNearClip = camera.getPerspectiveNearClip();
					float perspectiveFarClip = camera.getPerspectiveFarClip();

					if (ImGui::DragFloat("FOV", &perspectiveFOV))
					{
						camera.setPerspectiveFOV(glm::radians(perspectiveFOV));
					}


					if (ImGui::DragFloat("Near", &perspectiveNearClip))
					{
						camera.setPerspectiveNearClip(perspectiveNearClip);
					}

					if (ImGui::DragFloat("Far", &perspectiveFarClip))
					{
						camera.setPerspectiveFarClip(perspectiveFarClip);
					}
				}

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthographicSize = camera.getOrthographicSize();
					float orthographicNearClip = camera.getOrthographicNearClip();
					float orthographicFarClip = camera.getOrthographicFarClip();
					
					if (ImGui::DragFloat("Size", &orthographicSize))
					{
						camera.setOrthographicsSize(orthographicSize);
					}


					if (ImGui::DragFloat("Near", &orthographicNearClip))
					{
						camera.setOrthographicNearClip(orthographicNearClip);
					}

					if (ImGui::DragFloat("Far", &orthographicFarClip))
					{
						camera.setOrthographicFarClip(orthographicFarClip);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}
}