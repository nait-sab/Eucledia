#pragma once

#include <glm/glm.hpp>

namespace Eucledia
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const { return _position; }
		void setPosition(const glm::vec3& position) { _position = position; recalculateViewMatrix(); }

		float getRotation() const { return _rotation; }
		void setRotation(float rotation) { _rotation = rotation; recalculateViewMatrix(); }

		const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return _viewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return _viewProjectionMatrix; }

	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjectionMatrix;

		glm::vec3 _position = { 0, 0, 0 };
		float _rotation = 0;

		void recalculateViewMatrix();
	};
}

