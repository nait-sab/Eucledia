#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Eucledia
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setInt(const std::string& name, const int& value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual const std::string& getName() const = 0;

		static ref<Shader> create(const std::string& filepath);
		static ref<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const ref<Shader>& shader);
		void add(const ref<Shader>& shader);
		ref<Shader> load(const std::string& filepath);
		ref<Shader> load(const std::string& name, const std::string& filepath);

		ref<Shader> get(const std::string& name);

		bool exists(const std::string& name);

	private:
		std::unordered_map<std::string, ref<Shader>> _shaders;
	};
}