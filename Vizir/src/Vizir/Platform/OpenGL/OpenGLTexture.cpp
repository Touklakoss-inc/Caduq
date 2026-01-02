#include "vzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Vizir
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, Format format, Type type)
		: m_Width(width), m_Height(height), m_Channels(GetChannels(format))
	{
		VZ_PROFILE_FUNC()

		m_Format = GetNativeFormat(format);
		m_FormatStride = GetNativeFormatStride(format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_FormatStride, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(m_RendererID, 1, m_FormatStride, m_Width, m_Height);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		VZ_PROFILE_FUNC()

		Texture::TextureData textureData = LoadFile(path);

		m_Width = textureData.width;
		m_Height = textureData.height;
		m_Channels = textureData.channels;

		m_Format = GetNativeFormat(textureData.format);
		m_FormatStride = GetNativeFormatStride(textureData.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTextureStorage2D(m_RendererID, 1, m_FormatStride, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		SetData(textureData.data.data(), m_Width * m_Height * textureData.channels);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		VZ_PROFILE_FUNC()

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		VZ_PROFILE_FUNC()

		VZ_CORE_ASSERT(size == m_Width * m_Height * m_Channels, "Data must cover all the texture");

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		VZ_PROFILE_FUNC()

		glBindTextureUnit(slot, m_RendererID);
	}

	unsigned int OpenGLTexture2D::GetNativeFormat(Texture::Format format)
	{
		return format == Texture::Format::R8G8B8 ? GL_RGB : GL_RGBA;
	}

	unsigned int OpenGLTexture2D::GetNativeFormatStride(Texture::Format format)
	{
		return format == Texture::Format::R8G8B8 ? GL_RGB8 : GL_RGBA8;
	}
}