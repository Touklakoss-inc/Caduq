#include "vzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Vizir
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, TextureType type)
	{
		m_Specifications = {
			type,
			format,
			width,
			height,
			GetChannels(format)
		};

		VZ_PROFILE_FUNC()

		m_NativeFormat = GetNativeFormat(m_Specifications.format);
		m_NativeFormatStride = GetNativeFormatStride(m_Specifications.format);
		m_NativeType = GetNativeType(m_Specifications.type);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_NativeFormatStride, m_Specifications.width, m_Specifications.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		VZ_PROFILE_FUNC()

		Texture::TextureData textureData = LoadFile(path);
		m_Specifications = textureData.specifications;

		m_NativeFormat = GetNativeFormat(m_Specifications.format);
		m_NativeFormatStride = GetNativeFormatStride(m_Specifications.format);
		m_NativeType = GetNativeType(m_Specifications.type);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTextureStorage2D(m_RendererID, 1, m_NativeFormatStride, m_Specifications.width, m_Specifications.height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		SetData(textureData.data.data(), m_Specifications.width * m_Specifications.height * m_Specifications.channels);

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

		VZ_CORE_ASSERT(size == m_Specifications.width * m_Specifications.height * m_Specifications.channels, "Data must cover all the texture");

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specifications.width, m_Specifications.height, m_NativeFormat, m_NativeType, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		VZ_PROFILE_FUNC()

		glBindTextureUnit(slot, m_RendererID);
	}

	unsigned int OpenGLTexture2D::GetNativeFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8G8B8:
			return GL_RGB;
		case TextureFormat::R8G8B8A8:
			return GL_RGBA;
		case TextureFormat::D24S8:
			return GL_DEPTH_STENCIL;
		default:
			VZ_CORE_ASSERT(false, "Texture format was not recognized"); return 0;
		}
	}

	unsigned int OpenGLTexture2D::GetNativeFormatStride(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8G8B8:
			return GL_RGB8;
		case TextureFormat::R8G8B8A8:
			return GL_RGBA8;
		case TextureFormat::D24S8:
			return GL_DEPTH24_STENCIL8;
		default:
			VZ_CORE_ASSERT(false, "Texture format was not recognized"); return 0;
		}
	}

	unsigned int OpenGLTexture2D::GetNativeType(TextureType type)
	{
		switch (type)
		{
		case TextureType::UINT:
			return GL_UNSIGNED_INT;		
		case TextureType::UBYTE:
				return GL_UNSIGNED_BYTE;
		case TextureType::UINT_UNORM:
			return 0;
		default:
			VZ_CORE_ASSERT(false, "Texture format was not recognized"); return 0;
		}
	}
}