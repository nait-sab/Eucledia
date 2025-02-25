#include <Eucledia.h>
#include <Eucledia/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Eucledia
{
	class App : public Application
	{
	public:
		App() : Application("Eucledia Editor")
		{
			pushLayer(new EditorLayer());
		}

		~App()
		{

		}
	};

	Application* createApplication()
	{
		return new App();
	}
}