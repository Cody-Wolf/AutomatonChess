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
#include"Model.h"

struct Model
{
	Model(unsigned int id, const glm::vec3& pos, float rot, const glm::vec3& scale, const glm::vec3& color)
	{
		this->id = id;
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
		this->color = color;
	}
	unsigned int id;
	glm::vec3 pos;
	float rot;
	glm::vec3 scale;
	glm::vec3 color;
};

class RenderManger
{
private:
	std::vector<Model> m_Models;
	std::vector<mModel> m_md;
	Program *l_shader, *t_shader, *m_shader;
public:
	void init()
	{
		l_shader = new Program("engine/render/shader/LightVertex.shader", "engine/render/shader/LightFragment.shader");
		t_shader = new Program("engine/render/shader/2dVertex.shader", "engine/render/shader/2dFragment.shader");
		m_shader = new Program("engine/render/shader/ModelVertex.shader", "engine/render/shader/ModelFragment.shader");
		m_md.emplace_back("engine/rely/Pawn.stl");
		m_md.emplace_back("engine/rely/Knight.stl");
		m_md.emplace_back("engine/rely/King.stl");
		m_md.emplace_back("engine/rely/Ball.stl");

	}
	~RenderManger()
	{
		delete l_shader;
		delete t_shader;
		delete m_shader;
	}
	void addRender(unsigned int id, const glm::vec3& pos, float rot, const glm::vec3& scale, const glm::vec3& color)
	{
		m_Models.emplace_back(id, pos, rot, scale, color);
	}
	void Render()
	{
		unsigned int dpfbo, dpt;
		glGenFramebuffers(1, &dpfbo);
		glGenTextures(1, &dpt);
		glBindTexture(GL_TEXTURE_2D, dpt);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindFramebuffer(GL_FRAMEBUFFER, dpfbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dpt, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glm::mat4 lview;
		{
			glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 50.0f, 50.0f), glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 proj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f);
			lview = proj * view;
		}
		{
			glViewport(0, 0, 4096, 4096);
			glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
			l_shader->Setuniform4m("lview", glm::value_ptr(lview));
			{
				VertexArray vao;
				VertexBuffer vbo;
				VertexBufferLayout layout;
				layout.Layout<float>(3, 0);
				layout.Layout<float>(3, 0);
				layout.Layout<float>(2, 0);
				float postion[] = {
					0.0f	,0.0f	,0.0f	,0.0f	,0.0f	,0.0f	,0.0f	,0.0f,
					100.0f	,0.0f	,0.0f	,0.0f	,0.0f	,0.0f	,1.0f	,0.0f,
					0.0f	,0.0f	,100.0f	,0.0f	,0.0f	,0.0f	,0.0f	,1.0f,
					100.0f	,0.0f	,0.0f	,0.0f	,0.0f	,0.0f	,1.0f	,0.0f,
					0.0f	,0.0f	,100.0f	,0.0f	,0.0f	,0.0f	,0.0f	,1.0f,
					100.0f	,0.0f	,100.0f	,0.0f	,0.0f	,0.0f	,1.0f	,1.0f
				};
				vbo.SetData(sizeof(postion), postion, GL_STATIC_DRAW);
				vao.Bind(vbo);
				vao.Bind(layout);
				l_shader->Setuniform4m("model", glm::value_ptr(glm::mat4(1.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			{
				for (auto model : m_Models)
				{
					float x = model.pos.x;
					float y = model.pos.z;
					float z = model.pos.y;
					glm::mat4 modeli(1.0f);
					modeli = glm::translate(modeli, glm::vec3(x, y, z));
					modeli = glm::scale(modeli, model.scale);
					modeli = glm::rotate(modeli, model.rot, glm::vec3(0.0f, 1.0f, 0.0f));
					l_shader->Setuniform4m("model", glm::value_ptr(modeli));
					m_md[model.id - 1].Draw(*l_shader);
				}
			}
		}
		glViewport(0, 0, 1920, 1080);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dpt);
		glm::mat4 view, proj;
		view = glm::lookAt(glm::vec3(50.0f, 30.0f, -20.0f), glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::perspective(glm::radians(75.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		{
			Texture map;
			map.Bind("engine/rely/floor.jpg");
			t_shader->Setuniform4m("lview", glm::value_ptr(lview));
			t_shader->Setuniform1i("dtex", 1);
			VertexArray vao;
			VertexBuffer vbo;
			VertexBufferLayout layout;
			layout.Layout<float>(3, 0);
			layout.Layout<float>(2, 0);
			float postion[] = {
				0.0f	,0.0f	,0.0f	,0.0f	,0.0f,
				100.0f	,0.0f	,0.0f	,1.0f	,0.0f,
				0.0f	,0.0f	,100.0f	,0.0f	,1.0f,
				100.0f	,0.0f	,0.0f	,1.0f	,0.0f,
				0.0f	,0.0f	,100.0f	,0.0f	,1.0f,
				100.0f	,0.0f	,100.0f	,1.0f	,1.0f
			};
			vbo.SetData(sizeof(postion), postion, GL_STATIC_DRAW);
			vao.Bind(vbo);
			vao.Bind(layout);
			t_shader->Setuniform4m("view", glm::value_ptr(view));
			t_shader->Setuniform4m("proj", glm::value_ptr(proj));
			t_shader->Setuniform1i("tex", 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		m_shader->Setuniform3f("cpos", 50.0f, 30.0f, -20.0f);
		m_shader->Setuniform3f("ldir", -1.0f, -1.0f, 0.0f);
		m_shader->Setuniform4m("lview", glm::value_ptr(lview));
		m_shader->Setuniform1i("dtex", 1);
		while (m_Models.size())
		{
			
			auto model = m_Models.back(); m_Models.pop_back();
			float x = model.pos.x;
			float y = model.pos.z;
			float z = model.pos.y;
			glm::mat4 modeli(1.0f);
			modeli = glm::translate(modeli, glm::vec3(x, y, z));
			modeli = glm::scale(modeli, model.scale);
			modeli = glm::rotate(modeli, model.rot, glm::vec3(0.0f, 1.0f, 0.0f));
			m_shader->Setuniform3f("color", model.color.r, model.color.g, model.color.b);
			m_shader->Setuniform4m("model", glm::value_ptr(modeli));
			m_shader->Setuniform4m("view", glm::value_ptr(view));
			m_shader->Setuniform4m("proj", glm::value_ptr(proj));
			m_md[model.id - 1].Draw(*m_shader);
		}
		glDeleteTextures(1, &dpt);
		glDeleteFramebuffers(1, &dpfbo);

	}
};

static RenderManger GlobalRenderManger;