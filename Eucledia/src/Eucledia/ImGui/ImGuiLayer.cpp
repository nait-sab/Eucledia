#include "euclediapch.h"
#include "Eucledia/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Eucledia/Core/Application.h"

// Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <ImGuizmo.h>

namespace Eucledia
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::onAttach()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF("assets/fonts/Quicksand/Quicksand-Bold.ttf", 20.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Quicksand/Quicksand-Regular.ttf", 20.f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		setDarkThemeColors();

		Application& app = Application::get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onEvent(Event& event)
	{
		if (_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event._handled |= event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event._handled |= event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::begin()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::end()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContext);
		}
	}

	void ImGuiLayer::setDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		// Background
		colors[ImGuiCol_WindowBg] = ImVec4{ .1f, .105f, .11f, 1.f };

		// Header
		colors[ImGuiCol_Header] = ImVec4{ .2f, .205f, .21f, 1.f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ .3f, .305f, .31f, 1.f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ .15f, .1505f, .151f, 1.f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ .2f, .205f, .21f, 1.f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ .3f, .305f, .31f, 1.f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ .15f, .1505f, .151f, 1.f };

		// Frame Background
		colors[ImGuiCol_FrameBg] = ImVec4{ .2f, .205f, .21f, 1.f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ .3f, .305f, .31f, 1.f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ .15f, .1505f, .151f, 1.f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ .15f, .1505f, .151f, 1.f };
		colors[ImGuiCol_TabHovered] = ImVec4{ .38f, .3805f, .381f, 1.f };
		colors[ImGuiCol_TabActive] = ImVec4{ .28f, .2805f, .281f, 1.f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ .15f, .1505f, .151f, 1.f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ .2f, .205f, .21f, 1.f };

		// Title Background
		colors[ImGuiCol_TitleBg] = ImVec4{ .15f, .1505f, .151f, 1.f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ .15f, .1505f, .151f, 1.f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ .95f, .1505f, .951f, 1.f };
	}
}