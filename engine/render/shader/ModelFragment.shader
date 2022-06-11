#version 330 core

uniform vec3 color;

in vec3 Normal;
in vec3 fragpos;
out vec4 Color;

uniform vec3 cpos;
uniform vec3 ldir;

void main()
{
	vec3 cdir = normalize(cpos - fragpos);
	vec3 normal = normalize(Normal);
	vec3 lidir = normalize(ldir);
	vec3 ambient = color * 0.2f;
	vec3 diffuse = max(dot(-lidir, normal),0.0f) * color;
	vec3 spec = pow(max(dot(normalize(cdir - lidir), normal), 0.0f), 32) * color;
	Color = vec4(spec + diffuse + ambient, 1.0f);
}