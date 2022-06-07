#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class IndexBuffer
{
private:
	unsigned int id;
	unsigned int size;
public:
	IndexBuffer()
	{
		glGenBuffers(1, &id);
		size = 0;
	}
	IndexBuffer(unsigned int sz, const void* data, unsigned int type)
	{
		glGenBuffers(1, &id);
		SetData(sz, data, type);
	}
	~IndexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	unsigned int Size() const
	{
		return size;
	}

	void SetData(unsigned int sz, const void* data, unsigned int type)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, data, type);
		size = sz;
	}
};