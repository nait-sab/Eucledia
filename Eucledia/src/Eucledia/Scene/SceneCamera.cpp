#include "euclediapch.h"
#include "Eucledia/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eucledia
{
	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;
		recalculateProjection();
	}
	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		recalculateProjection();
	}

	void SceneCamera::recalculateProjection()
	{
		float orthoLeft = -_orthographicSize * _aspectRatio * .5f;
		float orthoRight = _orthographicSize * _aspectRatio * .5f;
		float orthoBottom = -_orthographicSize * .5f;
		float orthoTop = _orthographicSize * .5f;

		_projection = glm::ortho(
			orthoLeft,
			orthoRight,
			orthoBottom,
			orthoTop,
			_orthographicNear,
			_orthographicFar
		);
	}
}