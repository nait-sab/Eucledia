#include "euclediapch.h"
#include "Eucledia/Core/Input.h"

#include "Eucledia/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Eucledia
{
	bool Input::isKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xPosition, yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);
		return { (float)xPosition, (float)yPosition };
	}

	float Input::getMouseX()
	{
		auto [x, y] = getMousePosition();
		return x;
	}

	float Input::getMouseY()
	{
		auto [x, y] = getMousePosition();
		return y;
	}
}