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

group "Dependencies"
	include "Eucledia/vendor/GLFW"
	include "Eucledia/vendor/Glad"
	include "Eucledia/vendor/imgui"
	include "Eucledia/vendor/glm"

group ""

project "Eucledia"
	location "Eucledia"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "euclediapch.h"
	pchsource "Eucledia/src/euclediapch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/sdplog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS",
			"EUCLEDIA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		runtime "Release"
		symbols "on"
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
	staticruntime "Off"

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
		cppdialect "C++17"
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
		symbols "on"
		optimize "on"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/utf-8"
