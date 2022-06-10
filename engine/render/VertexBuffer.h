#pragma once

#include<vector>
#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

template<typename T>struct TypeForGL {};
template<>struct TypeForGL<float> { static constexpr unsigned int value = GL_FLOAT; };
template<>struct TypeForGL<int> { static constexpr unsigned int value = GL_INT; };
template<>struct TypeForGL<unsigned int> { static constexpr unsigned int value = GL_UNSIGNED_INT; };
template<>struct TypeForGL<bool> { static constexpr  int value = GL_BOOL; };
template<>struct TypeForGL<char> { static constexpr unsigned int value = GL_BYTE; };
template<>struct TypeForGL<unsigned char> { static constexpr unsigned int value = GL_UNSIGNED_BYTE; };

struct VertexAttrib
{
	unsigned int count, size, gltype, normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexAttrib> attrib;
	unsigned int sum;
public:
	VertexBufferLayout()
	{
		sum = 0;
	}

	template<typename T>
	void Layout(unsigned int count, unsigned int normalized)
	{
		attrib.push_back({ count,sizeof(T),TypeForGL<T>::value,normalized });;
		sum += sizeof(T) * count;
	}

	void Bind() const
	{
		unsigned int off = 0;
		for (unsigned int i = 0; i < attrib.size(); ++i)
		{
			glVertexAttribPointer(i, attrib[i].count, attrib[i].gltype, attrib[i].normalized, sum, (void*)off);
			glEnableVertexAttribArray(i);
			off += attrib[i].size * attrib[i].count;
		}
	}
};

class VertexBuffer
{
private:
	unsigned int id;

public:
	VertexBuffer()
	{
		glGenBuffers(1, &id);
	}
	VertexBuffer(unsigned int size, const void* data, unsigned int type)
	{
		glGenBuffers(1, &id);
		SetData(size, data, type);
	}
	VertexBuffer(VertexBuffer&& vb)
	{
		id = vb.id;
		vb.id = 0;
	}
	~VertexBuffer()
	{
		if(id)
			glDeleteBuffers(1, &id);
	}

	void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SetData(unsigned int size, const void* data, unsigned int type)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size, data, type);
	}
};