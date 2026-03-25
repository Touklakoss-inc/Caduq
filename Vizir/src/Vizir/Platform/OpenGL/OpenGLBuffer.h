#pragma once

#include "Renderer/Buffer.h"

namespace Vizir
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

		virtual void SetContent(uint32_t offset, uint32_t size, void* data) override;

		virtual uint32_t GetSize() override { return m_Size; }
		virtual uint32_t GetVertexCount() override { return m_Size / m_Layout.GetStride(); }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
		uint32_t m_Size;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetContent(uint32_t offset, uint32_t size, void* data) override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}
