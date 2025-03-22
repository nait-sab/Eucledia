#include "euclediapch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Eucledia
{
	static const std::filesystem::path _assetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : _currentDirectory(_assetsPath)
	{
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (_currentDirectory != std::filesystem::path(_assetsPath))
		{
			if (ImGui::Button("<-"))
			{
				_currentDirectory = _currentDirectory.parent_path();
			}
		}

		for (auto& element : std::filesystem::directory_iterator(_currentDirectory))
		{
			const auto& path = element.path();
			auto relativePath = std::filesystem::relative(path, _assetsPath);
			std::string filenameString = relativePath.filename().string();

			if (element.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					_currentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{
				}
			}
		}

		ImGui::End();
	}
}