#pragma once

#include "Eucledia.h"

class Sandbox2D : public Eucledia::Layer
{
public:
	Sandbox2D();

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Eucledia::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Eucledia::Event& event) override;

private:
	Eucledia::OrthographicCameraController _cameraController;

	// Temp
	Eucledia::ref<Eucledia::VertexArray> _squareVA;
	Eucledia::ShaderLibrary _shaderLibrary;
	Eucledia::ref<Eucledia::Framebuffer> _frameBuffer;
	Eucledia::ref<Eucledia::Texture2D> _texture;

	glm::vec4 _imguiColor;
};

