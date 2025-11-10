#pragma once

#include <memory>
#include "Buffer.h"

namespace Vizir
{
	enum PrimitiveType { NONE = 0, POINTS = 1, LINES = 2, LINE_STRIP = 3, LINE_LOOP = 4, TRIANGLES = 5, TRIANGLE_STRIP = 6, TRIANGLE_FAN = 7 };

	class VertexArray
	{
	public:

		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual void SetPrimitiveType(PrimitiveType primitiveType) = 0;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual unsigned int GetNativePrimitiveType() const = 0;

		static Ref<VertexArray> Create();
	};
}
