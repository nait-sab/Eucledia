include "./vendor/premake/premake_customization/solution_items.lua"
include "dependencies.lua"

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