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

    EditorLayer::EditorLayer() : Layer("EditorLayer"), _cameraController(16.f / 9.f), _imguiColor({ 0.8f, 0.2f, 0.2f, 1.f })
    {
    }

    void EditorLayer::onAttach()
    {
        EUCLEDIA_PROFILE_FUNCTION();

        _texture = Texture2D::create("assets/textures/default.png");

        FrameBufferSpecification spec;
        spec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        spec.width = 1600;
        spec.height = 900;
        _frameBuffer = Framebuffer::create(spec);

        _activeScene = createRef<Scene>();

        _editorCamera = EditorCamera(30.f, 1.778f, .1f, 1000.f);

        auto square = _activeScene->createEntity("Square");
        square.addComponent<SpriteRendererComponent>(glm::vec4{ .85f, .14f, .8f, 1.f });
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

        if (_viewportSize.x > 0.f && _viewportSize.y > 0.f && (specification.width != _viewportSize.x || specification.height != _viewportSize.y))
        {
            _frameBuffer->resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _cameraController.onResize(_viewportSize.x, _viewportSize.y);
            _editorCamera.setViewportSize(_viewportSize.x, _viewportSize.y);
            _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        }

        if (_viewportFocused)
        {
            _cameraController.onUpdate(ts);
        }

        _editorCamera.onUpdate(ts);

        Renderer2D::resetStats();

        _frameBuffer->bind();

        RenderCommand::setClearColor({ .15f, .15f, .15f, 1.f });
        RenderCommand::clear();

        _frameBuffer->clearAttachment(1, -1);

        _activeScene->onUpdateEditor(ts, _editorCamera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= _viewportBounds[0].x;
        my -= _viewportBounds[0].y;
        glm::vec2 viewportSize = _viewportBounds[1] - _viewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
           int pixelData = _frameBuffer->readPixel(1, mouseX, mouseY);
           _hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, _activeScene.get());
        }

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
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
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
            ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
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
        _contentBrowserPanel.onImGuiRender();

        ImGui::Begin("Stats");

        std::string name = "None";

        if (_hoveredEntity)
        {
            name = _hoveredEntity.getComponent<TagComponent>().tag;
        }

        ImGui::Text("Hovered Entity : %s", name.c_str());

        auto stats = Renderer2D::getStats();

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });

        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        _viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        _viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!_viewportFocused && !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(
            _frameBuffer->getColorAttachmentRendererID(), 
            ImVec2{ _viewportSize.x, _viewportSize.y }, 
            ImVec2{ 0.f, 1.f }, 
            ImVec2{ 1.f, 0.f }
        );

        Entity selectedEntity = _sceneHierarchyPanel.getSelectedEntity();

        if (selectedEntity && _guizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(
                _viewportBounds[0].x, _viewportBounds[0].y,
                _viewportBounds[1].x - _viewportBounds[0].x,
                _viewportBounds[1].y - _viewportBounds[0].y
            );

            /*auto cameraEntity = _activeScene->getPrimaryCameraEntity();
            const auto& camera = cameraEntity.getComponent<CameraComponent>().camera;
            const glm::mat4& cameraProjection = camera.getProjection();
            glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());*/

            const glm::mat4& cameraProjection = _editorCamera.getProjection();
            glm::mat4 cameraView = _editorCamera.getViewMatrix();

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
                glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
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
        _editorCamera.onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressedEvent>(EUCLEDIA_BIND_EVENT_FN(EditorLayer::onKeyPressed));
        dispatcher.dispatch<MouseButtonPressedEvent>(EUCLEDIA_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
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
                if (!ImGuizmo::IsUsing())
                {
                    _guizmoType = -1;
                }

                break;
            case EUCLEDIA_KEY_W:
                if (!ImGuizmo::IsUsing())
                {
                    _guizmoType = ImGuizmo::OPERATION::TRANSLATE;
                }

                break;
            case EUCLEDIA_KEY_E:
                if (!ImGuizmo::IsUsing())
                {
                    _guizmoType = ImGuizmo::OPERATION::ROTATE;
                }

                break;
            case EUCLEDIA_KEY_R:
                if (!ImGuizmo::IsUsing())
                {
                    _guizmoType = ImGuizmo::OPERATION::SCALE;
                }

                break;
        }

        return false;
    }

    bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        if (event.getMouseButton() == EUCLEDIA_MOUSE_BUTTON_LEFT && 
            _viewportHovered && 
            !ImGuizmo::IsOver() && 
            !Input::isKeyPressed(EUCLEDIA_KEY_LEFT_ALT)
        ) {
            _sceneHierarchyPanel.setSelectedEntity(_hoveredEntity);
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