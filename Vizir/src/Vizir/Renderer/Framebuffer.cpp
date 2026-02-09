#include "vzpch.h"
#include "Framebuffer.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Vizir
{
	Ref<Framebuffer> Framebuffer::Create(FramebufferSpecifications specs)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLFramebuffer>(specs);
			case RendererAPI::API::None:		VZ_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		VZ_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
