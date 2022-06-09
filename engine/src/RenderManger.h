#pragma once

#include<vector>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/matrix.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"../render/VertexArray.h"
#include"../render/VertexBuffer.h"
#include"../render/Shader.h"
#include"../render/Texture.h"

struct Model
{
	Model(unsigned int id, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const glm::vec3& color)
	{
		this->id = id;
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
		this->color = color;
	}
	unsigned int id;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 color;
};

static std::vector<float> position[5]=
{
	{
		-0.5f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,

		 0.5f,-0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	},
	{
		-0.5f, 0.0f, 0.5f,//1
		 0.5f, 0.0f, 0.5f,//2
		 0.0f, 1.0f, 0.0f,//3

		 0.5f, 0.0f, 0.5f,//2
		 0.0f, 0.0f,-0.5f,//4
		 0.0f, 1.0f, 0.0f,//3

		 0.0f, 0.0f,-0.5f,//4
		-0.5f, 0.0f, 0.5f,//1
		 0.0f, 1.0f, 0.0f//3

	},
	{
		-0.5f, 0.0f, 0.5f,//1
		 0.5f, 0.0f, 0.5f,//2
		 0.0f, 1.0f, 0.0f,//5

		 0.5f, 0.0f, 0.5f,//2
		 0.5f, 0.0f,-0.5f,//3
		 0.0f, 1.0f, 0.0f,//5

		 0.5f, 0.0f,-0.5f,//3
		-0.5f, 0.0f,-0.5f,//4
		 0.0f, 1.0f, 0.0f,//5

		-0.5f, 0.0f,-0.5f,//4
		-0.5f, 0.0f, 0.5f,//1
		 0.0f, 1.0f, 0.0f//5
	},
	{
		-0.4f, 0.0f, 0.5f,//1
		 0.4f, 0.0f, 0.5f,//2
		 0.0f, 1.0f, 0.0f,//6

		 0.4f, 0.0f, 0.5f,//2
		 0.5f, 0.0f, 0.0f,//3
		 0.0f, 1.0f, 0.0f,//6

		 0.5f, 0.0f, 0.0f,//3
		 0.0f, 0.0f,-0.5f,//4
		 0.0f, 1.0f, 0.0f,//6

		 0.0f, 0.0f,-0.5f,//4
		-0.5f, 0.0f, 0.0f,//5
		 0.0f, 1.0f, 0.0f,//6

		-0.5f, 0.0f, 0.0f,//5
		-0.4f, 0.0f, 0.5f,//1
		 0.0f, 1.0f, 0.0f,//6
	},
	{
		-0.5f, 0.5f, 0.5f,//1
		-0.5f,-0.5f, 0.5f,//2
		 0.5f,-0.5f, 0.5f,//3

		 0.5f,-0.5f, 0.5f,//3
		 0.5f, 0.5f, 0.5f,//4
		-0.5f, 0.5f, 0.5f,//1

		 0.5f, 0.5f, 0.5f,//4
		 0.5f,-0.5f, 0.5f,//3
		 0.5f,-0.5f,-0.5f,//6

		 0.5f,-0.5f,-0.5f,//6
		 0.5f, 0.5f,-0.5f,//7
		 0.5f, 0.5f, 0.5f,//4

		 0.5f, 0.5f,-0.5f,//7
		 0.5f,-0.5f,-0.5f,//6
		-0.5f,-0.5f,-0.5f,//5

		-0.5f, 0.5f,-0.5f,//8
		 0.5f, 0.5f,-0.5f,//7
		-0.5f,-0.5f,-0.5f,//5

		-0.5f, 0.5f,-0.5f,//8
		-0.5f,-0.5f,-0.5f,//5
		-0.5f,-0.5f, 0.5f,//2

		-0.5f, 0.5f,-0.5f,//8
		-0.5f,-0.5f, 0.5f,//2
		-0.5f, 0.5f, 0.5f,//1

		-0.5f, 0.5f,-0.5f,//8
		 0.5f, 0.5f, 0.5f,//4
		 0.5f, 0.5f,-0.5f,//7

		-0.5f, 0.5f,-0.5f,//8
		-0.5f, 0.5f, 0.5f,//1
		 0.5f, 0.5f, 0.5f,//4

		-0.5f,-0.5f,-0.5f,//5
		 0.5f,-0.5f,-0.5f,//6
		-0.5f,-0.5f, 0.5f,//2

		 0.5f,-0.5f,-0.5f,//6
		 0.5f,-0.5f, 0.5f,//3
		-0.5f,-0.5f, 0.5f//2
	}
};

class RenderManger
{
private:
	std::vector<Model> m_Models;
public:
	void addRender(unsigned int id, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const glm::vec3& color)
	{
		m_Models.emplace_back(id, pos, rot, scale, color);
	}
	void Render()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view, proj;
		view = glm::lookAt(glm::vec3(50.0f, 30.0f, 0.0f), glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::perspective(glm::radians(75.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		{
			Texture map;
			map.Bind("engine/rely/floor.jpg");
			Program shader("engine/render/shader/2dVertex.shader", "engine/render/shader/2dFragment.shader");
			VertexArray vao;
			VertexBuffer vbo;
			VertexBufferLayout layout;
			layout.Layout<float>(3, 0);
			layout.Layout<float>(2, 0);
			float postion[] = {
				0.0f,0.0f,0.0f,0.0f,0.0f,
				100.0f,0.0f,0.0f,1.0f,0.0f,
				0.0f,0.0f,100.0f,0.0f,1.0f,
				100.0f,0.0f,0.0f,1.0f,0.0f,
				0.0f,0.0f,100.0f,0.0f,1.0f,
				100.0f,0.0f,100.0f,1.0f,1.0f
			};
			vbo.SetData(sizeof(postion), postion, GL_STATIC_DRAW);
			vao.Bind(vbo);
			vao.Bind(layout);
			shader.Setuniform4m("view", glm::value_ptr(view));
			shader.Setuniform4m("proj", glm::value_ptr(proj));
			shader.Setuniform1i("tex", 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		Program shader("engine/render/shader/3dVertex.shader", "engine/render/shader/3dFragment.shader");
		while (m_Models.size())
		{
			auto model = m_Models.back(); m_Models.pop_back();
			float x = model.pos.x;
			float y = model.pos.z;
			float z = model.pos.y;

			glm::mat4 modeli(1.0f);
			modeli = glm::translate(modeli, glm::vec3(x, y, z));
			modeli = glm::scale(modeli, model.scale);
			VertexArray vao;
			VertexBuffer vbo;
			VertexBufferLayout layout;
			layout.Layout<float>(3, 0);
			vbo.SetData(position[model.id].size() * sizeof(float), position[model.id].data(), GL_STATIC_DRAW);
			vao.Bind(vbo);
			vao.Bind(layout);
			shader.Setuniform3f("color", model.color.r, model.color.g, model.color.b);
			shader.Setuniform4m("model", glm::value_ptr(modeli));
			shader.Setuniform4m("view", glm::value_ptr(view));
			shader.Setuniform4m("proj", glm::value_ptr(proj));
			glDrawArrays(GL_TRIANGLES, 0, position[model.id].size());
			

		}
	}
};

static RenderManger GlobalRenderManger;