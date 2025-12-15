#pragma once

#include <string>

#include "Vizir/Core/Core.h"


namespace Vizir
{
	class Texture
	{
	public:
		enum Type { UINT }; // Should be defined at another location
		enum Format { R8G8B8, R8G8B8A8 };

		struct TextureData {
			Type type;
			Format format;
			uint32_t width, height, channels;
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

		static uint32_t GetChannels(Format format);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, Format format = Format::R8G8B8, Type type = Type::UINT);
		static Ref<Texture2D> Create(const std::string& path);
	};
}
