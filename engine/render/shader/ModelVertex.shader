#version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texcoord;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	Normal = normalize(mat3(transpose(inverse(view * model))) * normal);
}