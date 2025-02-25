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

		glm::vec4 _imguiColor;
	};
}

