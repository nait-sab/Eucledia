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
IncludeDir["Glad"] = "Eucledia/vendor/Glad/include"
IncludeDir["ImGui"] = "Eucledia/vendor/imgui"

include "Eucledia/vendor/GLFW"
include "Eucledia/vendor/Glad"
include "Eucledia/vendor/imgui"

project "Eucledia"
	location "Eucledia"
	kind "SharedLib"
	language "C++"

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
		"%{IncludeDir.ImGui}"
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
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"EUCLEDIA_PLATFORM_WNDOWS",
			"EUCLEDIA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "EUCLEDIA_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		buildoptions "/MD"
		symbols "On"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		buildoptions "/MD"
		symbols "On"

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
		buildoptions "/MDd"
		symbols "ON"

	filter "configurations:Release"
		defines "EUCLEDIA_RELEASE"
		buildoptions "/MD"
		symbols "ON"

	filter "configurations:Dist"
		defines "EUCLEDIA_DIST"
		buildoptions "/MD"
		symbols "ON"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/utf-8"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/utf-8"
