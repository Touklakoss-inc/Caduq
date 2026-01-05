#pragma once

#include "Texture.h"

namespace Vizir
{
	struct FramebufferSpecifications
	{
		uint32_t width, height;
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(FramebufferSpecifications specs);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetAttachmentNativeID() const = 0;
	};
}
