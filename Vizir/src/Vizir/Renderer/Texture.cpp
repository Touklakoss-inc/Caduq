#include "vzpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vizir
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	VZ_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(width, height);
		}

		VZ_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	VZ_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
		}

		VZ_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}