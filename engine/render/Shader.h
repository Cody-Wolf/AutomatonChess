#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<sstream>
#include<fstream>
#include<unordered_map>

static std::string GetSource(const char* path)
{
	std::ifstream file(path);
	std::ostringstream ss;
	//std::cout << file.rdbuf() << std::endl;
	ss << file.rdbuf();
	return ss.str();
}

struct Shader
{
	unsigned int id;
	Shader(const char* path, unsigned int type)
	{
		id = glCreateShader(type);
		std::cout << path << std::endl;
		std::string source = GetSource(path);
		std::cout << source << std::endl;
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);
		int succes;
		glGetShaderiv(type, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			char infolog[512];
			glGetShaderInfoLog(type, 512, NULL, infolog);
			std::cout << "ERROR:SHADER:" << (type == GL_VERTEX_SHADER ? "VERTEX:" : "FRAGMENT:") << infolog << std::endl;
		}
	}
	~Shader()
	{
		glDeleteShader(id);
	}
};

class Program
{
private:
	std::unordered_map<std::string, int> locations;
	unsigned int id;
	int Findname(const std::string& name)
	{
		Bind();
		int location;
		if (locations.count(name))
			location = locations[name];
		else
		{
			location = glGetUniformLocation(id, name.c_str());
			locations[name] = location;
		}
		return location;
	}
public:
	Program(const char* VertexPath = 0, const char* FragmentPath = 0)
	{
		id = glCreateProgram();
		if (VertexPath)
		{
			Shader shader(VertexPath, GL_VERTEX_SHADER);
			glAttachShader(id, shader.id);
		}
		if (FragmentPath)
		{
			Shader shader(FragmentPath, GL_FRAGMENT_SHADER);
			glAttachShader(id, shader.id);
		}
		if (VertexPath && FragmentPath)
		{
			Link();
		}
	}
	~Program()
	{
		glDeleteProgram(id);
	}

	void Attach(const Shader& shader)
	{
		glAttachShader(id, shader.id);
	}
	void Attach(const char* path, unsigned int type)
	{
		Shader shader(path, type);
		glAttachShader(id, shader.id);
	}
	void Link() const
	{
		glLinkProgram(id);
		int succes;
		glGetProgramiv(id, GL_LINK_STATUS, &succes);
		if (!succes)
		{
			char infoLog[512];
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "PROGRAM:LINK:" << infoLog << std::endl;
		}
	}

	void Bind() const
	{
		glUseProgram(id);
	}
	void Unbind() const
	{
		glUseProgram(0);
	}

	void Setuniform4m(const std::string& name, const float* data)
	{
		int location = Findname(name);
		if (location == -1)
		{
			std::cout << "统一变量未找到:" << name << std::endl;
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, data);
	}
	void Setuniform4f(const std::string& name, float f0, float f1, float f2, float f3)
	{
		int location = Findname(name);
		if (location == -1)
		{
			std::cout << "统一变量未找到:" << name << std::endl;
			return;
		}
		glUniform4f(location, f0, f1, f2, f3);
	}
	void Setuniform3f(const std::string& name, float f0, float f1, float f2)
	{
		int location = Findname(name);
		if (location == -1)
		{
			std::cout << "统一变量未找到:" << name << std::endl;
			return;
		}
		glUniform3f(location, f0, f1, f2);
	}
	void Setuniform1i(const std::string& name, int i0)
	{
		int location = Findname(name);
		if (location == -1)
		{
			std::cout << "统一变量未找到:" << name << std::endl;
			return;
		}
		glUniform1i(location, i0);
	}
	void Setuniform1f(const std::string& name, float f0)
	{
		int location = Findname(name);
		if (location == -1)
		{
			std::cout << "统一变量未找到:" << name << std::endl;
			return;
		}
		glUniform1f(location, f0);
	}
};