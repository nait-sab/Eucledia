#include "euclediapch.h"
#include "Eucledia/Core/Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Eucledia
{
	scope<Window> Window::create(const WindowProps& props)
	{
		return createScope<WindowsWindow>(props);
	}
}
