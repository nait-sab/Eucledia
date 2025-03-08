#pragma once

#include "Eucledia.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Eucledia
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		void onUpdate(Timestep ts) override;
		virtual void onImGuiRender() override;
		void onEvent(Event& event) override;

		bool onKeyPressed(KeyPressedEvent& event);

		void newScene();
		void openScene();
		void saveSceneAs();

	private:
		OrthographicCameraController _cameraController;

		// Temp
		ref<VertexArray> _squareVA;
		ShaderLibrary _shaderLibrary;
		ref<Framebuffer> _frameBuffer;
		ref<Texture2D> _texture;

		ref<Scene> _activeScene;
		Entity _squareEntity, _cameraEntity;

		bool _primaryCamera = true;

		bool _viewportFocused = false, _viewportHovered = false;
		glm::vec2 _viewportSize = { 0, 0 };

		glm::vec4 _imguiColor;

		int _guizmoType = -1;

		SceneHierarchyPanel _sceneHierarchyPanel;
	};
}

