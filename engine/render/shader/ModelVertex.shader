#version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texcoord;

out vec3 Normal;
out vec3 fragpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 lview;
out vec4 frag_lpos;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	Normal = mat3(transpose(inverse(view * model))) * normal;
	fragpos = vec3(model * vec4(position, 1.0f));
	frag_lpos = lview * model * vec4(position, 1.0f);
}