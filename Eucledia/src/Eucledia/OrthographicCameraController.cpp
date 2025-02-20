#include "euclediapch.h"
#include "OrthographicCameraController.h"

#include "Eucledia/Input.h"
#include "Eucledia/KeyCodes.h"

namespace Eucledia
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _aspectRatio(aspectRatio), _rotation(rotation),
		_camera(-_aspectRatio * _zoomLevel, _aspectRatio* _zoomLevel, -_zoomLevel, _zoomLevel)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(EUCLEDIA_KEY_A))
		{
			_cameraPosition.x -= _cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(EUCLEDIA_KEY_D))
		{
			_cameraPosition.x += _cameraTranslationSpeed * ts;
		}

		if (Input::isKeyPressed(EUCLEDIA_KEY_W))
		{
			_cameraPosition.y += _cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(EUCLEDIA_KEY_S))
		{
			_cameraPosition.y -= _cameraTranslationSpeed * ts;
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

			_camera.setRotation(_cameraRotation);
		}
		
		_camera.setPosition(_cameraPosition);
		_cameraTranslationSpeed = _zoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseScrolledEvent>(EUCLEDIA_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(EUCLEDIA_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& event)
	{
		_zoomLevel -= event.getYOffset() * .25;
		_zoomLevel = std::max(_zoomLevel, .25f);
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& event)
	{
		_aspectRatio = (float)event.getWidth() / (float)event.getHeight();
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}
}