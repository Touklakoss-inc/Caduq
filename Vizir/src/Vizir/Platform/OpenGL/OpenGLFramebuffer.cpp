#include "vzpch.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Vizir
{
	Vizir::OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecifications specs)
		: m_Specifications(specs)
	{
		VZ_PROFILE_FUNC();

		Init();
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

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		VZ_PROFILE_FUNC();

		m_Specifications.width = width;
		m_Specifications.height = height;

		Cleanup();
		Init();
	}

	void OpenGLFramebuffer::Init()
	{
		VZ_PROFILE_FUNC();

		glCreateFramebuffers(1, &m_RendererID);
		Bind();
		GenerateAttachments();

		VZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

		Unbind();
	}

	void OpenGLFramebuffer::Cleanup()
	{
		VZ_PROFILE_FUNC();

		glDeleteFramebuffers(1, &m_RendererID);
		// Textures are disposed automatically
	}

	void OpenGLFramebuffer::GenerateAttachments()
	{
		VZ_PROFILE_FUNC();

		// Create color attachement
		m_ColorAttachment = Texture2D::Create(m_Specifications.width, m_Specifications.height, TextureFormat::R8G8B8A8, TextureType::UINT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment->GetID(), 0);

		// Clear color attachment
		glDrawBuffer(GL_COLOR_ATTACHMENT0); //Only need to do this once.
		uint32_t clearColor[4] = { 0, 0, 0, 0 };
		glClearBufferuiv(GL_COLOR, 0, clearColor);

		// Create render buffer
		m_DepthStencilAttachment = Texture2D::Create(m_Specifications.width, m_Specifications.height, TextureFormat::D24S8, TextureType::UINT_UNORM);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment->GetID());
	}
}
