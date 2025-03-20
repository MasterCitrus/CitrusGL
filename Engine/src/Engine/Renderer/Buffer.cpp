#include "cglpch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace CitrusGL
{
	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
	{
		glCreateBuffers(1, &bufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}