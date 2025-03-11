#include "euclediapch.h"
#include "EditorCamera.h"

#include "Eucledia/Core/Input.h"
#include "Eucledia/Core/KeyCodes.h"
#include "Eucledia/Core/MouseButtonCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Eucledia
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: _fov(fov), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip),
		Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		updateView();
	}

	void EditorCamera::updateProjection()
	{
		_aspectRatio = _viewportWidth / _viewportHeight;
		_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _nearClip, _farClip);
	}

	void EditorCamera::updateView()
	{
		// _yaw = _pitch = 0.f; // To lock camera rotation if needed

		_position = calculatePosition();
		glm::quat orientation = getOrientation();
		_viewMatrix = glm::translate(glm::mat4(1.f), _position) * glm::toMat4(orientation);
		_viewMatrix = glm::inverse(_viewMatrix);
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
		float x = std::min(_viewportWidth / 1000.f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(_viewportHeight / 1000.f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::rotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::zoomSpeed() const
	{
		float distance = _distance * .2f;
		distance = std::max(distance, 0.f);
		
		float speed = distance * distance;
		speed = std::min(speed, 100.f);

		return speed;
	}

	void EditorCamera::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(EUCLEDIA_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::getMouseX(), Input::getMouseY() };
			glm::vec2 delta = (mouse - _initialMousePosition) * .003f;
			_initialMousePosition = mouse;

			if (Input::isMouseButtonPressed(EUCLEDIA_MOUSE_BUTTON_MIDDLE))
			{
				mousePan(delta);
			}
			else if (Input::isMouseButtonPressed(EUCLEDIA_MOUSE_BUTTON_LEFT))
			{
				mouseRotate(delta);
			}
			else if (Input::isMouseButtonPressed(EUCLEDIA_MOUSE_BUTTON_RIGHT))
			{
				mouseZoom(delta.y);
			}
		}

		updateView();
	}

	void EditorCamera::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(EUCLEDIA_BIND_EVENT_FN(EditorCamera::onMouseScroll));
	}

	bool EditorCamera::onMouseScroll(MouseScrolledEvent& event)
	{
		float delta = event.getYOffset() * .1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		_focalPoint += -getRightDirection() * delta.x * xSpeed * _distance;
		_focalPoint += getUpDirection() * delta.y * ySpeed * _distance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.f : 1.f;
		_yaw += yawSign * delta.x * rotationSpeed();
		_pitch += delta.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		_distance -= delta * zoomSpeed();

		if (_distance < 1.f)
		{
			_focalPoint += getForwardDirection();
			_distance = 1.f;
		}
	}

	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.f, 0.f, 0.f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.f, 0.f, -1.f));
	}

	glm::vec3 EditorCamera::calculatePosition() const
	{
		return _focalPoint - getForwardDirection() * _distance;
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-_pitch, -_yaw, 0.f));
	}
}