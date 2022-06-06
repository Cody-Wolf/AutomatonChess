#pragma once

#include<vector>

#include<glm/glm.hpp>


struct Model
{
	unsigned int id;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 color;
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

	}
}GlobalRenderManger;