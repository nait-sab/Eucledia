#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Eucledia/Scene/SceneSerializer.h"
#include "Eucledia/Utils/PlatformUtils.h"
#include "Eucledia/Math/Math.h"

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

        auto square = _activeScene->createEntity("Square");
        square.addComponent<SpriteRendererComponent>(glm::vec4{ .85, .14, .8, 1 });
        _squareEntity = square;

        _cameraEntity = _activeScene->createEntity("Camera");
        _cameraEntity.addComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            void onUpdate(Timestep ts)
            {
                auto& translation = getComponent<TransformComponent>().translation;
                float speed = 5.f;

                if (Input::isKeyPressed(EUCLEDIA_KEY_A))
                {
                    translation.x -= speed * ts;
                }

                if (Input::isKeyPressed(EUCLEDIA_KEY_D))
                {
                    translation.x += speed * ts;
                }

                if (Input::isKeyPressed(EUCLEDIA_KEY_W))
                {
                    translation.y += speed * ts;
                }

                if (Input::isKeyPressed(EUCLEDIA_KEY_S))
                {
                    translation.y -= speed * ts;
                }
            }
        };

        _cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
        _sceneHierarchyPanel.setContext(_activeScene);

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
            _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        }

        if (_viewportFocused)
        {
            _cameraController.onUpdate(ts);
        }

        Renderer2D::resetStats();

        _frameBuffer->bind();

        RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
        RenderCommand::clear();

        _activeScene->onUpdate(ts);

        _frameBuffer->unbind();
    }

    void EditorLayer::onImGuiRender()
    {
        EUCLEDIA_PROFILE_FUNCTION();

        static bool dockspaceEnable = true;
        static bool opt_fullscreen = true;
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
        {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceEnable, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWindowSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWindowSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    newScene();
                }

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    openScene();
                }

                if (ImGui::MenuItem("Save as", "Ctrl+Shift+S"))
                {
                    saveSceneAs();
                }

                if (ImGui::MenuItem("Exit"))
                {
                    Application::get().close();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        _sceneHierarchyPanel.onImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        ImGui::Begin("Viewport");

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!_viewportFocused && !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(
            _frameBuffer->getColorAttachmentRendererID(), 
            ImVec2{ _viewportSize.x, _viewportSize.y }, 
            ImVec2{ 0, 1 }, 
            ImVec2{ 1, 0 }
        );

        Entity selectedEntity = _sceneHierarchyPanel.getSelectedEntity();

        if (selectedEntity && _guizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            auto cameraEntity = _activeScene->getPrimaryCameraEntity();
            const auto& camera = cameraEntity.getComponent<CameraComponent>().camera;
            const glm::mat4& cameraProjetion = camera.getProjection();
            glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

            auto& transformComponent = selectedEntity.getComponent<TransformComponent>();
            glm::mat4 transform = transformComponent.getTransform();

            bool snap = Input::isKeyPressed(EUCLEDIA_KEY_LEFT_CONTROL);
            float snapValue = .5f;

            if (_guizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.f;
            }

            float snapValues[3] = { snapValue, snapValue, snapValue };
                
            ImGuizmo::Manipulate(
                glm::value_ptr(cameraView), glm::value_ptr(cameraProjetion),
                (ImGuizmo::OPERATION)_guizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr
            );

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::decomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - transformComponent.rotation;
                transformComponent.translation = translation;
                transformComponent.rotation += deltaRotation;
                transformComponent.scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event)
    {
        _cameraController.onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressedEvent>(EUCLEDIA_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent& event)
    {
        if (event.getRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::isKeyPressed(EUCLEDIA_KEY_LEFT_CONTROL) || Input::isKeyPressed(EUCLEDIA_KEY_RIGHT_CONTROL);
        bool shift = Input::isKeyPressed(EUCLEDIA_KEY_LEFT_SHIFT) || Input::isKeyPressed(EUCLEDIA_KEY_RIGHT_SHIFT);

        switch (event.getKeyCode())
        {
            case EUCLEDIA_KEY_N:
            {
                if (control)
                {
                    newScene();
                }

                break;
            }
            case EUCLEDIA_KEY_O:
            {
                if (control)
                {
                    openScene();
                }

                break;
            }
            case EUCLEDIA_KEY_S:
            {
                if (control && shift)
                {
                    saveSceneAs();
                }

                break;
            }
            case EUCLEDIA_KEY_A:
                _guizmoType = -1;
                break;
            case EUCLEDIA_KEY_W:
                _guizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case EUCLEDIA_KEY_E:
                _guizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case EUCLEDIA_KEY_R:
                _guizmoType = ImGuizmo::OPERATION::SCALE;
                break;
        }

        return false;
    }

    void EditorLayer::newScene()
    {
        _activeScene = createRef<Scene>();
        _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        _sceneHierarchyPanel.setContext(_activeScene);
    }

    void EditorLayer::openScene()
    {
        std::string filepath = FileDialogs::openFile("Eucledia Scene (*.eucledia)\0*.eucledia\0");

        if (!filepath.empty())
        {
            _activeScene = createRef<Scene>();
            _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _sceneHierarchyPanel.setContext(_activeScene);
            SceneSerializer serializer(_activeScene);
            serializer.deserialize(filepath);
        }
    }

    void EditorLayer::saveSceneAs()
    {
        std::string filepath = FileDialogs::saveFile("Eucledia Scene (*.eucledia)\0*.eucledia\0");

        if (!filepath.empty())
        {
            SceneSerializer serializer(_activeScene);
            serializer.serialize(filepath);
        }
    }
}