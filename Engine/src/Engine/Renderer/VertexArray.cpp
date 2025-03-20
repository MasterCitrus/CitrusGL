#include "cglpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace CitrusGL
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case CitrusGL::ShaderDataType::Float:
		case CitrusGL::ShaderDataType::Float2:
		case CitrusGL::ShaderDataType::Float3:
		case CitrusGL::ShaderDataType::Float4:
		case CitrusGL::ShaderDataType::Mat3:
		case CitrusGL::ShaderDataType::Mat4:
			return GL_FLOAT;
		case CitrusGL::ShaderDataType::Int:
		case CitrusGL::ShaderDataType::Int2:
		case CitrusGL::ShaderDataType::Int3:
		case CitrusGL::ShaderDataType::Int4:
			return GL_INT;
		case CitrusGL::ShaderDataType::Bool:
			return GL_BOOL;
		}

		CGL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;

	}

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &vertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &vertexArrayID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(vertexArrayID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CGL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Cannot use a vertex buffer without a layout.");

		glBindVertexArray(vertexArrayID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
								  ShaderDataTypeToOpenGLBaseType(element.type),
								  element.normalised ? GL_TRUE : GL_FALSE,
								  layout.GetStride(),
								  (const void*)element.offset);
			index++;
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(vertexArrayID);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}
}
