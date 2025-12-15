#pragma once

#include "Renderer/Texture.h"

namespace Vizir
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, Format format, Type type);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t Getheight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;

		virtual uint32_t GetID() const override { return m_RendererID; }
	private:
		static unsigned int GetNativeFormat(Texture::Format format);
		static unsigned int GetNativeFormatStride(Texture::Format format);

		uint32_t m_Width, m_Height, m_Channels;
		uint32_t m_RendererID;
		unsigned int m_Format, m_FormatStride;
	};
}
