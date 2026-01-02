#include "vzpch.h"
#include "Framebuffer.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Vizir
{
	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLFramebuffer>(width, height);
			case RendererAPI::API::None:		VZ_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		VZ_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
