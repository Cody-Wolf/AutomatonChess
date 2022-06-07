#pragma once

#include<iostream>
#include<stb/stb_image.h>

struct Image
{
	int width, height, n;
	unsigned char* data;
	Image()
	{

	}

	Image(const char* path)
	{
		Open(path);
	}
	~Image()
	{
		stbi_image_free(data);
	}

	void Open(const char* path)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(path, &width, &height, &n, 0);
		if (!data)
		{
			std::cout << path << ":´ò¿ªÊ§°Ü" << std::endl;
		}
	}
};