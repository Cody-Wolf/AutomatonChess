#pragma once


#include"RenderManger.h"

class Object
{
private:
	unsigned int m_model;
	glm::vec3 m_scale;
public:
	Object(unsigned int id, const glm::vec3& scale) :m_scale(scale), m_model(id) {  }
	void draw(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& color)
	{
		GlobalRenderManger.addRender(m_model, pos, rot, m_scale, color);
	}
};

static Object makeThreeObject(const glm::vec3& scale = glm::vec3(0.05f))
{
	return Object(1, scale);
}
static Object makeFourObject(const glm::vec3& scale = glm::vec3(3))
{
	return Object(2, scale);
}
static Object makeFiveObject(const glm::vec3& scale = glm::vec3(3))
{
	return Object(3, scale);
}
static Object makeCubeObject(const glm::vec3& scale = glm::vec3(3))
{
	return Object(4, scale);
}