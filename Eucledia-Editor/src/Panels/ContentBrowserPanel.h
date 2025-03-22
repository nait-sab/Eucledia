#pragma once

#include <filesystem>

namespace Eucledia
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

	private:
		std::filesystem::path _currentDirectory;
	};
}

