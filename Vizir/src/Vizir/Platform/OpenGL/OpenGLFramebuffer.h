#pragma once

#include "Renderer/Framebuffer.h"

namespace Vizir
{
	class OpenGLFramebuffer: public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecifications specs);
		~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t GetAttachmentNativeID() const override { return m_ColorAttachment->GetID(); }
	private: 
		void Init();
		void Cleanup();
		void GenerateAttachments();

		FramebufferSpecifications m_Specifications{ };
		uint32_t m_RendererID{ 0 };

		Ref<Texture2D> m_ColorAttachment{ };
		Ref<Texture2D> m_DepthStencilAttachment{ };
	};
}
