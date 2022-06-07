#version 330 core

uniform vec3 color;

out Color;

int main()
{
	Color = color;
}