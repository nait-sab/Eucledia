VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Eucledia/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Eucledia/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Eucledia/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Eucledia/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Eucledia/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Eucledia/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Eucledia/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Eucledia/vendor/ImGuizmo"