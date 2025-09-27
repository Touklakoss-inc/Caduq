#include "jepch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>

// temp
#include "glm/ext.hpp"

namespace Judas_Engine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		JE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		JE_PROFILE_FUNC()

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// assets/texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		JE_PROFILE_FUNC()

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		JE_PROFILE_FUNC()

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			JE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		JE_PROFILE_FUNC()

		std::unordered_map<GLenum, std::string> shaderSources;

		char* typeToken = "#type";
		size_t typeTokenLength = std::strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != source.npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			JE_CORE_ASSERT(eol != source.npos, "Synthax error");
			size_t begin = pos + typeTokenLength + 1; // There must be only one space between #type and vertex/fragment
			std::string type = source.substr(begin, eol - begin);
			JE_CORE_ASSERT(ShaderTypeFromString(type), "Invalide shader type specified");

			size_t nextLinePos = source.find_first_of("\r\n", eol);
			JE_CORE_ASSERT(nextLinePos != std::string::npos, "Synthax error");
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		JE_PROFILE_FUNC()

		// Get a program object.
		uint32_t program = glCreateProgram();
		JE_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIds;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(kv.first);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)kv.second.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				JE_CORE_ERROR("{0}", infoLog.data());
				JE_CORE_ASSERT(false, "Shader compilation failure");

				// In this simple program, we'll just leave
				return;
			}

			glAttachShader(program, shader);
			glShaderIds[glShaderIDIndex++] = shader;
		}

		// Link our program
		m_RendererID = program;
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for(auto id : glShaderIds)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			JE_CORE_ERROR("{0}", infoLog.data());
			JE_CORE_ASSERT(false, "Shader linking failed");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIds)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}
	}

	void OpenGLShader::Bind() const
	{
		JE_PROFILE_FUNC()

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		JE_PROFILE_FUNC()

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const char* name, int value)
	{
		JE_PROFILE_FUNC()

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const char* name, float value)
	{
		JE_PROFILE_FUNC()

			UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const char* name, const glm::vec3& value)
	{
		JE_PROFILE_FUNC()

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const char* name, const glm::vec4& value)
	{
		JE_PROFILE_FUNC()

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& mat)
	{
		JE_PROFILE_FUNC()

		UploadUniformMat4(name, mat);
	}

	void OpenGLShader::UploadUniformInt(const char* name, int value)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const char* name, float value)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const char* name, const glm::vec2& values)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const char* name, const glm::vec3& values)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniform3f(location, values.x, values.y, values.z);
	}
	
	void OpenGLShader::UploadUniformFloat4(const char* name, const glm::vec4& values)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const char* name, const glm::mat3& mat)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniformMatrix3fv(location, 1, false, glm::value_ptr(mat));
	}

	void OpenGLShader::UploadUniformMat4(const char* name, const glm::mat4& mat)
	{
		unsigned int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
			return;

		glUniformMatrix4fv(location, 1, false, glm::value_ptr(mat));
	}
}