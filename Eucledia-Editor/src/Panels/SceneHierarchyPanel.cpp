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

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				_context->createEntity("Empty Entity");
			}

			ImGui::EndPopup();
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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			_selectionContext = entity;
		}

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			_context->destroyEntity(entity);

			if (_selectionContext == entity)
			{
				_selectionContext = {};
			}
		}
	}

	static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.f, float columnWidth = 100.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .1f, .8f, .1f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .1f, 1.f, .2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .1f, .8f, .1f, 1.f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .1f, .1f, .8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .1f, .1f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .1f, .1f, .8f, 1.f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}

		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, .1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFonction>
	static void drawComponent(const std::string& name, Entity entity, UIFonction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodesFlags = ImGuiTreeNodeFlags_DefaultOpen 
			| ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding;

		if (entity.hasComponent<T>())
		{
			auto& component = entity.getComponent<T>();
			bool removeComponent = false;
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodesFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * .5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.removeComponent<T>();
			}
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
			
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Sprite"))
			{
				_selectionContext.addComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Camera"))
			{
				_selectionContext.addComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		drawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			glm::vec3 rotation = glm::degrees(component.rotation);
			drawVec3Control("Translation", component.translation);
			drawVec3Control("Rotation", rotation);
			drawVec3Control("Scale", component.scale, 1.f);
			component.rotation = glm::radians(rotation);
		});

		drawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

		drawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.camera;

			ImGui::Checkbox("Primary", &component.primary);

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

				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
		});
	}
}