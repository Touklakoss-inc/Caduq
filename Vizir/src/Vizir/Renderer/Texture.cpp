#include "vzpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "stb_image.h"

namespace Vizir
{
	Texture::TextureData Texture::LoadFile(const std::string& path)
	{
		VZ_TRACE("Called LoadFile");

		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		std::vector<unsigned char> textureData;
		{
			VZ_PROFILE_SCOPE("Loading data - OpenGLTexture2D::OpenGLTexture2D")

			// Extract data and size
			stbi_uc* ptr;
			ptr = stbi_load(path.c_str(), &width, &height, &channels, 0);
			VZ_CORE_ASSERT(ptr, "Failed to load image");

			// Copy data to vector
			size_t size = static_cast<size_t>(width * height * channels); // ========================================================================
			textureData.resize(size);																			// /!\ Only because the texture is composed of unsigned char of size 1 byte
			memcpy(textureData.data(), ptr, size);												// ========================================================================

			// cleanup temp memory
			stbi_image_free(ptr);
		}

		// Extract format and type
		TextureType type = TextureType::UBYTE;
		TextureFormat format;
		if (channels == 4)
		{
			format = TextureFormat::R8G8B8A8;
		}
		else if (channels == 3)
		{
			format = TextureFormat::R8G8B8;
		}
		else
		{
			VZ_ASSERT(false, "Only 3 or 4 channels are supported, detected {}.", channels)
		}

		return Texture::TextureData{
			type, format,
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height),
			static_cast<uint32_t>(channels),
			textureData
		};
	}

	uint32_t Texture::GetChannels(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8G8B8:
			return 3;
		case TextureFormat::R8G8B8A8:
		case TextureFormat::D24S8:
			return 4;
		default:
			VZ_CORE_ASSERT(false, "Unknown texture format"); return 0;
		}
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureFormat format, TextureType type)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	VZ_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(width, height, format, type);
		}

		VZ_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	VZ_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
		}

		VZ_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}