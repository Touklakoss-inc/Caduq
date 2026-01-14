#include "vzpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Vizir
{
	static unsigned int GetNativePolygonMode(RendererAPI::PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
		case RendererAPI::PolygonMode::POINT:
			return GL_POINT;
		case RendererAPI::PolygonMode::LINE:
			return GL_LINE;
		case RendererAPI::PolygonMode::FILL:
			return GL_FILL;
		default:
			VZ_CORE_ASSERT(false, "Unknown Polygon mode"); return 0;
		}
	}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewPort(int x, int y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements((GLenum)vertexArray->GetNativePrimitiveType(), vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::SetPolygonMode(PolygonMode polygonMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GetNativePolygonMode(polygonMode));
	}

	void OpenGLRendererAPI::SetPointSize(float pointSize)
	{
		glPointSize(pointSize);
	}

	void OpenGLRendererAPI::SetLineWidth(float lineWidth)
	{
		glLineWidth(lineWidth);
	}

	void OpenGLRendererAPI::EnablePrimitiveRestart()
	{
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(UINT32_MAX);
	}
}
