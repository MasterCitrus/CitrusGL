#pragma once

#include "Engine/Renderer/Buffer.h"

#include <memory>

namespace CitrusGL
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return vertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }

	private:
		uint32_t vertexArrayID;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
}