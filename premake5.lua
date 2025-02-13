workspace "Eucledia"
	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Eucledia/vendor/GLFW/include"

include "Eucledia/vendor/GLFW"

project "Eucledia"
	location "Eucledia"
	kind "SharedLib"
	language "C++"
	staticruntime "on"

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
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS",
			"EUCLEDIA_BUILD_DLL"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		symbols "ON"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		symbols "ON"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		symbols "ON"

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

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Eucledia/src",
		"Eucledia/vendor/sdplog/include"
	}

	links {
		"Eucledia"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS"
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		symbols "ON"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		symbols "ON"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		symbols "ON"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/utf-8"
