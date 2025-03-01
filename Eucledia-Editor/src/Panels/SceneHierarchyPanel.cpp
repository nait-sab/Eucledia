#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

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
}