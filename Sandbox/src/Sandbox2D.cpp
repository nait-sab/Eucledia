#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func) : _name(name), _func(func), _stopped(false)
	{
		_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!_stopped)
		{
			stop();
		}
	}

	void stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		_stopped = true;

		float duration = (end - start) * 0.001f;
		_func({ _name, duration });
	}

private:
	const char* _name;
	Fn _func;
	std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
	bool _stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { _profileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _cameraController(16.f / 9.f)
{
}

void Sandbox2D::onAttach()
{
	_texture = Eucledia::Texture2D::create("assets/textures/default.png");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Eucledia::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::onUpdate");

	_cameraController.onUpdate(ts);

	Eucledia::RenderCommand::setClearColor({ .15f, .15f, .15f, 1 });
	Eucledia::RenderCommand::clear();

	Eucledia::Renderer2D::beginScene(_cameraController.getCamera());
	Eucledia::Renderer2D::drawQuad({ -1, 0 }, { .8, .8 }, { 0.8, 0.2, 0.2, 1 });
	Eucledia::Renderer2D::drawQuad({ .5, -.5 }, { .5, .75 }, { 0.8, 0.2, 0.8, 1 });
	Eucledia::Renderer2D::drawQuad({ .5, .5, -.1 }, { .75, .7 }, _texture);
	Eucledia::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));

	for (auto& result : _profileResults)
	{
		char label[50];
		strcpy(label, "  %.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::onEvent(Eucledia::Event& event)
{
	_cameraController.onEvent(event);
}
