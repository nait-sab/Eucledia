#include "euclediapch.h"
#include "Eucledia/Renderer/OrthographicCameraController.h"

#include "Eucledia/Core/Input.h"
#include "Eucledia/Core/KeyCodes.h"

namespace Eucledia
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _aspectRatio(aspectRatio), _rotation(rotation),
		_camera(-_aspectRatio * _zoomLevel, _aspectRatio* _zoomLevel, -_zoomLevel, _zoomLevel)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		if (Input::isKeyPressed(EUCLEDIA_KEY_A))
		{
			_cameraPosition.x -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y -= sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(EUCLEDIA_KEY_D))
		{
			_cameraPosition.x += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y += sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}

		if (Input::isKeyPressed(EUCLEDIA_KEY_W))
		{
			_cameraPosition.x += -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(EUCLEDIA_KEY_S))
		{
			_cameraPosition.x -= -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
			_cameraPosition.y -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
		}

		if (_rotation)
		{
			if (Input::isKeyPressed(EUCLEDIA_KEY_Q))
			{
				_cameraRotation += _cameraRotationSpeed * ts;
			}
			else if (Input::isKeyPressed(EUCLEDIA_KEY_E))
			{
				_cameraRotation -= _cameraRotationSpeed * ts;
			}

			if (_cameraRotation > 180)
			{
				_cameraRotation -= 360;
			}
			else if (_cameraRotation <= -180)
			{
				_cameraRotation += 360;
			}

			_camera.setRotation(_cameraRotation);
		}
		
		_camera.setPosition(_cameraPosition);
		_cameraTranslationSpeed = _zoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& event)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(EUCLEDIA_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(EUCLEDIA_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& event)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_zoomLevel -= event.getYOffset() * .25f;
		_zoomLevel = std::max(_zoomLevel, .25f);
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& event)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_aspectRatio = (float)event.getWidth() / (float)event.getHeight();
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}
}