#pragma once

#include "Eucledia.h"

class ExampleLayer : public Eucledia::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Eucledia::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Eucledia::Event& event) override;

private:
	Eucledia::ShaderLibrary _shaderLibrary;
	Eucledia::ref<Eucledia::VertexArray> _triangleVA, _squareVA;
	Eucledia::ref<Eucledia::Texture2D> _texture;

	Eucledia::OrthographicCameraController _cameraController;

	glm::vec4 _squareColor = { 0.8, 0.8, 0.8, 1.0 };
};

