#pragma once

#include "RendererAPI.h"

namespace Vizir
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewPort(int x, int y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
		inline static void SetPolygonMode(RendererAPI::PolygonMode polygonMode)
		{
			s_RendererAPI->SetPolygonMode(polygonMode);
		}
		inline static void SetPointSize(float pointSize)
		{
			s_RendererAPI->SetPointSize(pointSize);
		}
		inline static void SetLineWidth(float lineWidth)
		{
			s_RendererAPI->SetLineWidth(lineWidth);
		}
		inline static void EnablePrimitiveRestart()
		{
			s_RendererAPI->EnablePrimitiveRestart();
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
