#pragma once

#include "Eucledia/Renderer/Camera.h"

namespace Eucledia
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		SceneCamera();
		virtual ~SceneCamera() = default;

		void setPerspective(float fov, float nearClip, float farClip);
		void setOrthographic(float size, float nearClip, float farClip);

		void setViewportSize(uint32_t width, uint32_t height);

		float getPerspectiveFOV() const { return _perspectiveFOV; }
		float getPerspectiveNearClip() const { return _perspectiveNear; }
		float getPerspectiveFarClip() const { return _perspectiveFar; }
		void setPerspectiveFOV(float fov) { _perspectiveFOV = fov; recalculateProjection(); }
		void setPerspectiveNearClip(float nearClip) { _perspectiveNear = nearClip; recalculateProjection(); }
		void setPerspectiveFarClip(float farClip) { _perspectiveFar = farClip; recalculateProjection(); }

		float getOrthographicSize() const { return _orthographicSize; }
		float getOrthographicNearClip() const { return _orthographicNear; }
		float getOrthographicFarClip() const { return _orthographicFar; }
		void setOrthographicsSize(float size) { _orthographicSize = size; recalculateProjection(); }
		void setOrthographicNearClip(float nearClip) { _orthographicNear = nearClip; recalculateProjection(); }
		void setOrthographicFarClip(float farClip) { _orthographicFar = farClip; recalculateProjection(); }

		ProjectionType getProjectionType() const { return _protectionType; }
		void setProjectionType(ProjectionType type) { _protectionType = type; }

	private:
		ProjectionType _protectionType = ProjectionType::Orthographic;

		float _perspectiveFOV = glm::radians(45.f);
		float _perspectiveNear = 0.01f, _perspectiveFar = 1000.f;

		float _orthographicSize = 10.f;
		float _orthographicNear = -1.f, _orthographicFar = 1.f;
		float _aspectRatio = 0.f;

		void recalculateProjection();
	};
}