#version 330 core

uniform sampler2D tex;

in vec2 tcoord;
out vec4 Color;

void main()
{
	Color = texture(tex, tcoord);
}