#pragma once

#include "Camera.h"
#include "Eucledia/Core/Timestep.h"
#include "Eucledia/Events/Event.h"
#include "Eucledia/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Eucledia
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void onUpdate(Timestep ts);
		void onEvent(Event& event);

		float getDistance() const { return _distance; }
		float setDistance(float distance) { _distance = distance; }

		void setViewportSize(float width, float height) { _viewportWidth = width; _viewportHeight = height; updateProjection(); }

		const glm::mat4& getViewMatrix() const { return _viewMatrix; }
		glm::mat4 getViewProjection() const { return _projection * _viewMatrix; }

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3 getPosition() const { return _position; }
		glm::quat getOrientation() const;

		float getPitch() const { return _pitch; }
		float getYaw() const { return _yaw; }

	private:
		float _fov = 45.f, _aspectRatio = 1.778f, _nearClip = .1f, _farClip = 1000.f;

		glm::mat4 _viewMatrix;
		glm::vec3 _position = { 0.f, 0.f, 0.f };
		glm::vec3 _focalPoint = { 0.f, 0.f, 0.f };

		glm::vec2 _initialMousePosition = { 0.f, 0.f };

		float _distance = 10.f;
		float _pitch = 0.f, _yaw = 0.f;

		float _viewportWidth = 1600, _viewportHeight = 900;

		void updateProjection();
		void updateView();

		bool onMouseScroll(MouseScrolledEvent& event);

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec3 calculatePosition() const;

		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;
	};
}
