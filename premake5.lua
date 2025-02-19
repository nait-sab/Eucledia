workspace "Eucledia"
	architecture "x64"
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Eucledia/vendor/GLFW/include"
IncludeDir["Glad"] = "Eucledia/vendor/Glad/include"
IncludeDir["ImGui"] = "Eucledia/vendor/imgui"
IncludeDir["glm"] = "Eucledia/vendor/glm"
IncludeDir["stb_image"] = "Eucledia/vendor/stb_image"

group "Dependencies"
	include "Eucledia/vendor/GLFW"
	include "Eucledia/vendor/Glad"
	include "Eucledia/vendor/imgui"
	include "Eucledia/vendor/glm"

group ""

project "Eucledia"
	location "Eucledia"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "euclediapch.h"
	pchsource "Eucledia/src/euclediapch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CTR_SECURE_NO_WARNINGS"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/sdplog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS",
			"EUCLEDIA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		runtime "Release"
		optimize "on"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/utf-8"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Eucledia/vendor/sdplog/include",
		"Eucledia/src",
		"Eucledia/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"Eucledia"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS"
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		runtime "Release"
		optimize "on"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/utf-8"
