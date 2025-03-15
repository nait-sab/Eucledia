#include "euclediapch.h"
#include "Eucledia/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Eucledia/Core/Application.h"

namespace Eucledia
{
	std::string Eucledia::FileDialogs::openFile(const char* filter)
	{
		OPENFILENAMEA dialog;
		CHAR fileBuffer[260] = { 0 };
		CHAR currentDir[256] = { 0 };

		ZeroMemory(&dialog, sizeof(OPENFILENAME));
		dialog.lStructSize = sizeof(OPENFILENAME);
		dialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		dialog.lpstrFile = fileBuffer;
		dialog.nMaxFile = sizeof(fileBuffer);

		if (GetCurrentDirectoryA(256, currentDir))
		{
			dialog.lpstrInitialDir = currentDir;
		}

		dialog.lpstrFilter = filter; // filter = File extensions allowed
		dialog.nFilterIndex = 1;
		dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&dialog) == TRUE)
		{
			return dialog.lpstrFile;
		}

		return std::string();
	}

	std::string Eucledia::FileDialogs::saveFile(const char* filter)
	{
		OPENFILENAMEA dialog;
		CHAR fileBuffer[260] = { 0 };
		CHAR currentDir[256] = { 0 };

		ZeroMemory(&dialog, sizeof(OPENFILENAME));
		dialog.lStructSize = sizeof(OPENFILENAME);
		dialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
		dialog.lpstrFile = fileBuffer;
		dialog.nMaxFile = sizeof(fileBuffer);

		if (GetCurrentDirectoryA(256, currentDir))
		{
			dialog.lpstrInitialDir = currentDir;
		}

		dialog.lpstrFilter = filter; // filter = File extensions allowed
		dialog.nFilterIndex = 1;
		dialog.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		dialog.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&dialog) == TRUE)
		{
			return dialog.lpstrFile;
		}

		return std::string();
	}
}