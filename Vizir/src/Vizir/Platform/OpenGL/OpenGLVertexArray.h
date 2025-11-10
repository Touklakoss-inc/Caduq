#pragma once

#include "../../Renderer/VertexArray.h"

#include <vector>

typedef unsigned int GLenum;

namespace Vizir
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() {}

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual void SetPrimitiveType(PrimitiveType primitiveType) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const override { return m_VertexBuffer; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

		virtual unsigned int GetNativePrimitiveType() const override { return m_PrimitiveType; }

	private:
		Ref<VertexBuffer> m_VertexBuffer{};
		Ref<IndexBuffer> m_IndexBuffer{};
		
		GLenum m_PrimitiveType{};

		uint32_t m_RendererID{};
	};
}
