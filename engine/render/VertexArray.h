#pragma once

#include"IndexBuffer.h"
#include"VertexBuffer.h"

class VertexArray
{
private:
	unsigned int id;
	unsigned int size;
public:
	VertexArray()
	{
		glGenVertexArrays(1, &id);
		size = 0;
	}
	~VertexArray()
	{
		if(id)
			glDeleteVertexArrays(1, &id);
	}

	void Bind() const
	{
		glBindVertexArray(id);
	}
	void Bind(const VertexBuffer& vbo)
	{
		Bind();
		vbo.Bind();
	}
	void Bind(const IndexBuffer& ibo)
	{
		Bind();
		ibo.Bind();
		size = ibo.Size();
	}
	void Bind(const VertexBufferLayout& layout)
	{
		Bind();
		layout.Bind();
	}
	unsigned int Size()
	{
		return size;
	}
	void Unbind()
	{
		glBindVertexArray(0);
	}
};