#pragma once

#include "Eucledia/Core/Base.h"
#include "Eucledia/Core/Log.h"
#include "Eucledia/Scene/Scene.h"
#include "Eucledia/Scene/Entity.h"

namespace Eucledia
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const ref<Scene>& context);

		void setContext(const ref<Scene>& context);

		void onImGuiRender();

		Entity getSelectedEntity() const { return _selectionContext; }
		void setSelectedEntity(Entity entity);

	private:
		ref<Scene> _context;
		Entity _selectionContext;

		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);
	};
}

