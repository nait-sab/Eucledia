#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

namespace Eucledia
{

    EditorLayer::EditorLayer() : Layer("EditorLayer"), _cameraController(16.f / 9.f), _imguiColor({ 0.8, 0.2, 0.2, 1 })
    {
    }

    void EditorLayer::onAttach()
    {
        EUCLEDIA_PROFILE_FUNCTION();

        _texture = Texture2D::create("assets/textures/default.png");

        FrameBufferSpecification spec;
        spec.width = 1600;
        spec.height = 900;
        _frameBuffer = Framebuffer::create(spec);

        _activeScene = createRef<Scene>();

        auto square = _activeScene->createEntity("Green Square");
        square.addComponent<SpriteRendererComponent>(glm::vec4{ 0, 1, 0, 1 });
        _squareEntity = square;
    }

    void EditorLayer::onDetach()
    {
        EUCLEDIA_PROFILE_FUNCTION();
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        EUCLEDIA_PROFILE_FUNCTION();

        FrameBufferSpecification specification = _frameBuffer->getSpecification();

        if (_viewportSize.x > 0 && _viewportSize.y > 0 && (specification.width != _viewportSize.x || specification.height != _viewportSize.y))
        {
            _frameBuffer->resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _cameraController.onResize(_viewportSize.x, _viewportSize.y);
        }

        if (_viewportFocused)
        {
            _cameraController.onUpdate(ts);
        }

        Renderer2D::resetStats();

        _frameBuffer->bind();

        RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
        RenderCommand::clear();

        Renderer2D::beginScene(_cameraController.getCamera());
        _activeScene->onUpdate(ts);
        Renderer2D::endScene();

        _frameBuffer->unbind();
    }

    void EditorLayer::onImGuiRender()
    {
        EUCLEDIA_PROFILE_FUNCTION();

        static bool dockspaceEnable = true;
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
                    Application::get().close();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");

        auto stats = Renderer2D::getStats();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        if (_squareEntity)
        {
            ImGui::Separator();
            auto& tag = _squareEntity.getComponent<TagComponent>().tag;
            ImGui::Text("%s", tag.c_str());

            auto& squareColor = _squareEntity.getComponent<SpriteRendererComponent>().color;
            ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
            ImGui::Separator();
        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        ImGui::Begin("Viewport");

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!_viewportFocused || !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(
            _frameBuffer->getColorAttachmentRendererID(), 
            ImVec2{ _viewportSize.x, _viewportSize.y }, 
            ImVec2{ 0, 1 }, 
            ImVec2{ 1, 0 }
        );
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event)
    {
        _cameraController.onEvent(event);
    }
}