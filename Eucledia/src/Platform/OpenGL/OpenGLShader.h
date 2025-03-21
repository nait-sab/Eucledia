#pragma once

#include "Eucledia/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: Remove this ref
typedef unsigned int GLenum;

namespace Eucledia
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setInt(const std::string& name, const int& value) override;
		virtual void setIntArray(const std::string& name, const int* values, uint32_t count) override;
		virtual void setFloat(const std::string& name, const float value) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& values) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) override;

		virtual const std::string& getName() const override { return _name; }

		void uploadUniformInt(const std::string& name, const int& value);
		void uploadUniformIntArray(const std::string& name, const int* values, uint32_t count);
		void uploadUniformFloat(const std::string& name, const float& value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t _rendererID;
		std::string _name;

		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};
}