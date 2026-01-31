#include "vzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Vizir
{
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		}

		VZ_CORE_ASSERT(false, "Unknown ShaderDataType !");
		return GL_NONE;
	}

	GLenum PrimitiveTypeToOpenGLBaseType(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::NONE:							return GL_NONE;
		case PrimitiveType::POINTS:						return GL_POINTS;
		case PrimitiveType::LINES:						return GL_LINES;
		case PrimitiveType::LINE_STRIP:				return GL_LINE_STRIP;
		case PrimitiveType::LINE_LOOP:				return GL_LINE_LOOP;
		case PrimitiveType::TRIANGLES:				return GL_TRIANGLES;
		case PrimitiveType::TRIANGLE_STRIP:		return GL_TRIANGLE_STRIP;
		case PrimitiveType::TRIANGLE_FAN:			return GL_TRIANGLE_FAN;
		}

		VZ_CORE_ASSERT(false, "Unknown PrimitiveType !");
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		VZ_PROFILE_FUNC()

		glCreateVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		VZ_PROFILE_FUNC()

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		VZ_PROFILE_FUNC()

		VZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		int index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset)));
			index++;
		}

		m_VertexBuffer = vertexBuffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		VZ_PROFILE_FUNC()

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
	void OpenGLVertexArray::SetPrimitiveType(PrimitiveType primitiveType)
	{
		VZ_PROFILE_FUNC()

		m_PrimitiveType = PrimitiveTypeToOpenGLBaseType(primitiveType);
	}
}