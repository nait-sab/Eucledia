#include "euclediapch.h"
#include "Eucledia/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eucledia
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _projectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)), _viewMatrix(1.f)
	{
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
	{
		_projectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::rotate(glm::mat4(1.f), glm::radians(_rotation), glm::vec3(0, 0, 1));

		_viewMatrix = glm::inverse(transform);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}
}