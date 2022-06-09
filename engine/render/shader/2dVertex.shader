#version 330 core

layout(location = 0)in vec3 apos;
layout(location = 1)in vec2 tpos;

uniform mat4 view;
uniform mat4 proj;

out vec2 tcoord;

void main()
{
	gl_Position = proj * view * vec4(apos, 1.0f);
	tcoord = tpos;
}