#pragma once

#include <string>

#include "Vizir/Core/Core.h"


namespace Vizir
{
	enum TextureType {
		UINT,
		UINT_UNORM
	}; // Should be defined at another location

	enum TextureFormat {
		R8G8B8,
		R8G8B8A8,
		D24S8
	};

	struct TextureSpecifications {
		TextureType type;
		TextureFormat format;
		uint32_t width, height, channels;
	};

	class Texture
	{
	public:
		struct TextureData {
			TextureSpecifications specifications;
			std::vector<unsigned char> data;
		};

		~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t Getheight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetID() const = 0;
	protected:
		static TextureData LoadFile(const std::string& path);
		static uint32_t GetChannels(TextureFormat format);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureFormat format = TextureFormat::R8G8B8, TextureType type = TextureType::UINT);
		static Ref<Texture2D> Create(const std::string& path);
	};
}
