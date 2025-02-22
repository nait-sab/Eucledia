#include "euclediapch.h"
#include "Eucledia/Core/Input.h"
#include "Platform/Windows/WindowsInput.h"

namespace Eucledia
{
	scope<Input> Input::_instance = Input::create();

	scope<Input> Input::create()
	{
		return createScope<WindowsInput>();
	}
}