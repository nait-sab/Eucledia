#include "euclediapch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Eucledia
{
	ref<Shader> Shader::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return createRef<OpenGLShader>(filepath);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				EUCLEDIA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	void ShaderLibrary::add(const std::string& name, const ref<Shader>& shader)
	{
		EUCLEDIA_CORE_ASSERT(!exists(name), "Shader already exists");
		_shaders[name] = shader;
	}

	void ShaderLibrary::add(const ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	ref<Shader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		EUCLEDIA_CORE_ASSERT(exists(name), "Shader not found");
		return _shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name)
	{
		return _shaders.find(name) != _shaders.end();
	}
}