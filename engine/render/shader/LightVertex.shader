#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 lview;
uniform mat4 model;

void main()
{
	gl_Position = lview * model * vec4(position, 1.0f);
}