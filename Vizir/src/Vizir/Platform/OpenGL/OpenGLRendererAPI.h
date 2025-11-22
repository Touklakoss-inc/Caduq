#pragma once

#include "Renderer/RendererAPI.h"

namespace Vizir
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewPort(int x, int y, uint32_t width, uint32_t height) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		virtual void SetPointSize(float pointSize) override;
		virtual void SetLineWidth(float lineWidth) override;
		virtual void EnablePrimitiveRestart() override;

	};
}