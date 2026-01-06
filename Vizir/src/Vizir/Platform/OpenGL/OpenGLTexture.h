#pragma once

#include "Renderer/Texture.h"

namespace Vizir
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, TextureType type);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Specifications.width; }
		virtual uint32_t Getheight() const override { return m_Specifications.height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;

		virtual uint32_t GetID() const override { return m_RendererID; }
	private:
		static unsigned int GetNativeFormat(TextureFormat format);
		static unsigned int GetNativeFormatStride(TextureFormat format);
		static unsigned int GetNativeType(TextureType type);

		TextureSpecifications m_Specifications;
		uint32_t m_RendererID;
		unsigned int m_NativeFormat, m_NativeFormatStride, m_NativeType;
	};
}
