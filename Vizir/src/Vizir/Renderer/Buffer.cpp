#include "vzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Vizir
{
	VertexBuffer* Vizir::VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
			case RendererAPI::API::None:		VZ_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		VZ_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:	return new OpenGLIndexBuffer(indices, count);
			case RendererAPI::API::None:		VZ_ASSERT(false, "RendererAPI::None is not currently supported"); return nullptr;
		}

		VZ_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
