#pragma once

#include "Vizir/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Vizir
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual const std::string GetName() { return m_Name; }

		void Bind() const;
		void Unbind() const;

		virtual void SetInt(const char* name, int value) override;
		virtual void SetFloat(const char* name, float value) override;
		virtual void SetFloat3(const char* name, const glm::vec3& value) override;
		virtual void SetFloat4(const char* name, const glm::vec4& value) override;
		virtual void SetMat4(const char* name, const glm::mat4& mat) override;

		void UploadUniformInt(const char* name, int value);

		void UploadUniformFloat(const char* name, float value);
		void UploadUniformFloat2(const char* name, const glm::vec2& values);
		void UploadUniformFloat3(const char* name, const glm::vec3& values);
		void UploadUniformFloat4(const char* name, const glm::vec4& values);

		void UploadUniformMat3(const char* name, const glm::mat3& mat);
		void UploadUniformMat4(const char* name, const glm::mat4& mat);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		std::string m_Name;
		unsigned int m_RendererID;
	};
}

