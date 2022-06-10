#pragma once

#include<glm/glm.hpp>
#include<string>
#include<vector>
#include<iostream>
#include"../render/Shader.h"
#include"../render/VertexArray.h"
#include"../render/IndexBuffer.h"
#include"../render/VertexBuffer.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct mTexture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	void setUpMesh()
	{
		vao.Bind();
		VertexBufferLayout layout;
		layout.Layout<float>(3, 0);
		layout.Layout<float>(3, 0);
		layout.Layout<float>(2, 0);
		vbo.SetData(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		ibo.SetData(indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		vao.Bind(vbo);
		vao.Bind(layout);
		vao.Bind(ibo);
		vao.Unbind();
	}
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<mTexture> textures;
	Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<mTexture>textures) :vao(), vbo(), ibo()
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setUpMesh();
	}
	void Draw(Program& shader)
	{
		shader.Bind();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};

class mModel
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(const std::string& path)
	{
		Assimp::Importer port;
		const aiScene* scene = port.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}
	void processNode(aiNode* node, const aiScene* scene)
	{
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			auto t = processMesh(mesh, scene);

			meshes.emplace_back(std::get<0>(t), std::get<1>(t), std::get<2>(t));

		}
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	std::tuple<std::vector<Vertex>, std::vector<unsigned int>, std::vector<mTexture>> processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<mTexture> textures;

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f);
			}
			vertices.push_back(vertex);
		}
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}
		std::cout << indices.size() << "|||||||||||||||||||||||||||||||||\n";
		return { vertices, indices, textures };
	}
	std::vector<mTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{

	}
public:
	mModel(const char* path)
	{
#ifdef  _DEBUG
		std::cout << "Model¿ªÊ¼¼ÓÔØ" << std::endl;
#endif //  _DEBUG

		loadModel(path);
	}
	void Draw(Program& shader)
	{
		for (int i = 0; i < meshes.size(); ++i)
			meshes[i].Draw(shader);
	}
};