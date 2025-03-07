#pragma once

#include "Eucledia/Renderer/OrthographicCamera.h"
#include "Eucledia/Core/Timestep.h"

#include "Eucledia/Events/ApplicationEvent.h"
#include "Eucledia/Events/MouseEvent.h"

namespace Eucledia
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& event);

		void onResize(float width, float height);

		OrthographicCamera& getCamera() { return _camera; }
		const OrthographicCamera& getCamera() const { return _camera; }

		float getZoomLevel() const { return _zoomLevel; }
		void setZoomLevel(float zoomLevel) { _zoomLevel = zoomLevel; }

	private:
		float _aspectRatio;
		float _zoomLevel = 1;
		OrthographicCamera _camera;

		glm::vec3 _cameraPosition = { 0,0,0 };
		float _cameraRotation = 0;
		float _cameraTranslationSpeed = 5, _cameraRotationSpeed = 150;

		bool _rotation;

		bool onMouseScrolled(MouseScrolledEvent& event);
		bool onWindowResized(WindowResizeEvent& event);
	};
}

