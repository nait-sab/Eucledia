#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

	static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.f, float columnWidth = 100.f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .8f, .1f, .1f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.f, .2f, .2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .8f, .1f, .1f, 1.f });

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .1f, .8f, .1f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .1f, 1.f, .2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .1f, .8f, .1f, 1.f });

		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .1f, .1f, .8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .1f, .1f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .1f, .1f, .8f, 1.f });

		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
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
				auto& transformComponent = entity.getComponent<TransformComponent>();
				glm::vec3 rotation = glm::degrees(transformComponent.rotation);

				drawVec3Control("Translation", transformComponent.translation);
				drawVec3Control("Rotation", rotation);
				drawVec3Control("Scale", transformComponent.scale, 1.f);

				transformComponent.rotation = glm::radians(rotation);
				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& color = entity.getComponent<SpriteRendererComponent>().color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.getComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				ImGui::Checkbox("Primary", &cameraComponent.primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];

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