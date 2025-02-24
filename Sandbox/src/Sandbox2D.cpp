#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(16.f / 9.f), _imguiColor({ 0.8, 0.2, 0.2, 1 })
{
}

void Sandbox2D::onAttach()
{
	EUCLEDIA_PROFILE_FUNCTION();

	_texture = Eucledia::Texture2D::create("assets/textures/default.png");

    Eucledia::FrameBufferSpecification spec;
    spec.width = 1600;
    spec.height = 900;
    _frameBuffer = Eucledia::Framebuffer::create(spec);
}

void Sandbox2D::onDetach()
{
	EUCLEDIA_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Eucledia::Timestep ts)
{
	EUCLEDIA_PROFILE_FUNCTION();

	_cameraController.onUpdate(ts);

	Eucledia::Renderer2D::resetStats();

	{
		EUCLEDIA_PROFILE_SCOPE("Renderer clean");
        _frameBuffer->bind();
		Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
		Eucledia::RenderCommand::clear();
	}

	{
		EUCLEDIA_PROFILE_SCOPE("Renderer draw");

		static float rotation = 0.0f;
		rotation += ts * 40;

		Eucledia::Renderer2D::beginScene(_cameraController.getCamera());
		Eucledia::Renderer2D::drawRotatedQuad({ 1, 0 }, { .8, .8 }, -45, {0.5, 0.2, 0.6, 1});
		Eucledia::Renderer2D::drawQuad({ -1, 0 }, { .8, .8 }, _imguiColor);
		Eucledia::Renderer2D::drawQuad({ .5, -.5 }, { .5, .75 }, { 0.8, 0.2, 0.8, 1 });
		Eucledia::Renderer2D::drawQuad({ 0, 0, -0.1 }, { 20, 20 },  _texture, 10);
		Eucledia::Renderer2D::drawRotatedQuad({ -2, 0, 0 }, { 1, 1 }, rotation, _texture, 20);
		Eucledia::Renderer2D::endScene();

		Eucledia::Renderer2D::beginScene(_cameraController.getCamera());

		for (float y = -5.f; y < 5.f; y += .5)
		{
			for (float x = -5.f; x < 5.f; x += .5)
			{
				glm::vec4 color = { (x + 5.f) / 10.f, .4, (y + 5.f) / 10.f, 1 };
				Eucledia::Renderer2D::drawQuad({ x, y }, { .45, .45 }, color);
			}
		}

		Eucledia::Renderer2D::endScene();
        _frameBuffer->unbind();
	}
}

void Sandbox2D::onImGuiRender()
{
	EUCLEDIA_PROFILE_FUNCTION();

	static bool dockspaceEnable = true;
   
	if (dockspaceEnable)
	{
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceEnable, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Eucledia::Application::get().close();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        ImGui::Begin("Settings");

        auto stats = Eucledia::Renderer2D::getStats();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::ColorEdit4("Square Color", glm::value_ptr(_imguiColor));
        ImGui::Image(_frameBuffer->getColorAttachmentRendererID(), ImVec2{ 1600.f, 900.f });
        ImGui::End();
	}
    else 
    {
        ImGui::Begin("Settings");

        auto stats = Eucledia::Renderer2D::getStats();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::ColorEdit4("Square Color", glm::value_ptr(_imguiColor));
        ImGui::Image(_texture->getRendererID(), ImVec2{ 256, 256 });
        ImGui::End();
    }
}

void Sandbox2D::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
