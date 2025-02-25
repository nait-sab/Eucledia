#pragma once

#include "Eucledia.h"

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

	private:
		OrthographicCameraController _cameraController;

		// Temp
		ref<VertexArray> _squareVA;
		ShaderLibrary _shaderLibrary;
		ref<Framebuffer> _frameBuffer;
		ref<Texture2D> _texture;

		ref<Scene> _activeScene;
		Entity _squareEntity;
		Entity _cameraEntity;

		bool _viewportFocused = false, _viewportHovered = false;
		glm::vec2 _viewportSize = { 0, 0 };

		glm::vec4 _imguiColor;
	};
}

