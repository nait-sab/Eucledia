include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Eucledia"
	architecture "x86_64"
	startproject "Eucledia-Editor"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Eucledia/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Eucledia/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Eucledia/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Eucledia/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Eucledia/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Eucledia/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Eucledia/vendor/yaml-cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "Eucledia/vendor/GLFW"
	include "Eucledia/vendor/Glad"
	include "Eucledia/vendor/imgui"
	include "Eucledia/vendor/glm"
	include "Eucledia/vendor/yaml-cpp"
group ""

include "Eucledia"
include "Sandbox"
include "Eucledia-Editor"