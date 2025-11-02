#include "jepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Judas_Engine
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
			case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
