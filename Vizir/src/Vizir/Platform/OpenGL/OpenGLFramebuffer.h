#pragma once

#include "Renderer/Framebuffer.h"

namespace Vizir
{
	class OpenGLFramebuffer: public Framebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddColorAttachement(const Ref<Texture2D>& texture) override;
		virtual void Validate() override;
	private:
		void GenerateDepthStencilAttachment();

		uint32_t m_RendererID{ 0 };
		uint32_t m_DepthStencilRendererID{ 0 };

		uint32_t m_Width{ 0 }, m_Height{ 0 };
		std::vector<Ref<Texture2D>> m_ColorAttachments{};
	};
}
