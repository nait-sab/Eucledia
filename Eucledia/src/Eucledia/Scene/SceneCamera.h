#pragma once

#include "Eucledia/Renderer/Camera.h"

namespace Eucledia
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setViewportSize(uint32_t width, uint32_t height);

		float getOrthographicSize() const { return _orthographicSize; }
		void setOrthographicsSize(float size) { _orthographicSize = size; recalculateProjection(); }

	private:
		float _orthographicSize = 10.f;
		float _orthographicNear = -1.f, _orthographicFar = 1.f;
		float _aspectRatio = 0.f;

		void recalculateProjection();
	};
}