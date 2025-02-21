#include "euclediapch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Eucledia
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}

		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = readFile(filepath);
		auto shaderSources = preProcess(source);
		compile(shaderSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		_name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: _name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_rendererID);
	}

	std::string OpenGLShader::readFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream file(filepath, std::ios::in | std::ios::binary);

		if (file)
		{
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			if (size != -1)
			{
				result.resize(file.tellg());
				file.seekg(0, std::ios::beg);
				file.read(&result[0], result.size());
				file.close();
			}
			else
			{
				EUCLEDIA_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			EUCLEDIA_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t position = source.find(typeToken, 0);

		while (position != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", position);
			EUCLEDIA_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = position + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			EUCLEDIA_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
			size_t nextLinePosition = source.find_first_of("\r\n", position);
			EUCLEDIA_CORE_ASSERT(nextLinePosition != std::string::npos, "Syntax error");
			position = source.find(typeToken, nextLinePosition);
			shaderSources[ShaderTypeFromString(type)] = (position == std::string::npos)
				? source.substr(nextLinePosition)
				: source.substr(nextLinePosition, position - nextLinePosition);
		}

		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		EUCLEDIA_CORE_ASSERT(shaderSources.size() <= 2, "Only two shaders supported");
		std::array<GLenum, 2> glShaderIds;
		int glShaderIdIndex = 0;

		for (auto& keyValue : shaderSources)
		{
			GLenum type = keyValue.first;
			const std::string& source = keyValue.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceString = source.c_str();
			glShaderSource(shader, 1, &sourceString, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);
				EUCLEDIA_CORE_ERROR("{0}", infoLog.data());
				EUCLEDIA_CORE_ASSERT(false, "Shader compilation failure");
				break;
			}

			glAttachShader(program, shader);
			glShaderIds[glShaderIdIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);

			for (auto id : glShaderIds)
			{
				glDeleteShader(id);
			}

			EUCLEDIA_CORE_ERROR("{0}", infoLog.data());
			EUCLEDIA_CORE_ASSERT(false, "Shader link failure");
			return;
		}

		for (auto id : glShaderIds)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		_rendererID = program;
	}

	void OpenGLShader::bind() const
	{
		glUseProgram(_rendererID);
	}

	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, const int& value)
	{
		uploadUniformInt(name, value);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& values)
	{
		uploadUniformFloat3(name, values);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& values)
	{
		uploadUniformFloat4(name, values);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix)
	{
		uploadUniformMat4(name, matrix);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, const int& value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, const float& value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}