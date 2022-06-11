#version 330 core
layout(location = 0)in vec3 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 lview;
out vec4 frag_lpos;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	frag_lpos = lview * model * vec4(position, 1.0f);
}