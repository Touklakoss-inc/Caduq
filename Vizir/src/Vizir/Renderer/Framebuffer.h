#pragma once

#include "Texture.h"

namespace Vizir
{
	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(uint32_t width, uint32_t height);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddColorAttachement(const Ref<Texture2D>& texture) = 0;
		virtual void Validate() = 0;
	};
}
