#include "vzpch.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Vizir
{
	Vizir::OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		VZ_PROFILE_FUNC();

		glCreateFramebuffers(1, &m_RendererID);
	}

	Vizir::OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		VZ_PROFILE_FUNC();

		glDeleteFramebuffers(1, &m_RendererID);
	}

	void Vizir::OpenGLFramebuffer::Bind()
	{
		VZ_PROFILE_FUNC();

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void Vizir::OpenGLFramebuffer::Unbind()
	{
		VZ_PROFILE_FUNC();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AddColorAttachement(const Ref<Texture2D>& texture)
	{
		VZ_CORE_ASSERT(texture->GetWidth() == m_Width && texture->Getheight() == m_Height, "Framebuffer and texture size mismatch");

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size(), GL_TEXTURE_2D, texture->GetID(), 0);
		m_ColorAttachments.push_back(texture);
	}

	void OpenGLFramebuffer::Validate()
	{
		VZ_PROFILE_FUNC();

		// Implemented this way for convenience
		// In case of more specific usage, should be changed
		GenerateDepthStencilAttachment();

		VZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
	}

	void OpenGLFramebuffer::GenerateDepthStencilAttachment()
	{
		VZ_PROFILE_FUNC();

		glGenRenderbuffers(1, &m_DepthStencilRendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRendererID);
	}
}
