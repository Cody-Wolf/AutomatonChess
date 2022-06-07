#pragma once

#include"Image.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Texture
{
private:
	unsigned int id;
public:
	Texture()
	{
		glGenTextures(1, &id);
	}

	~Texture()
	{
		glDeleteTextures(1, &id);
	}


	void Bind(unsigned int socket = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + socket);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void Bind(const char* path, unsigned int socket = 0)
	{
		Bind(socket);
		Image image(path);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB - 3 + image.n, image.width, image.height, 0, GL_RGB - 3 + image.n, GL_UNSIGNED_BYTE, image.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Bind(const Image& image, unsigned int socket = 0)
	{
		Bind(socket);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB - 3 + image.n, image.width, image.height, 0, GL_RGB - 3 + image.n, GL_UNSIGNED_BYTE, image.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void Set()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
};