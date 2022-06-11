#version 330 core

uniform vec3 color;

in vec3 Normal;
in vec3 fragpos;
out vec4 Color;

uniform vec3 cpos;
uniform vec3 ldir;

in vec4 frag_lpos;
uniform sampler2D dtex;

void main()
{
	vec3 fraglpos = (frag_lpos.xyz / frag_lpos.w) * 0.5 + 0.5;
	float ldepth = texture(dtex, fraglpos.xy).r;
	float bias = 0.005;
	float cdepth = fraglpos.z;
	float shadow = cdepth - bias > ldepth ? 1.0 : 0.0f;
	vec3 cdir = normalize(cpos - fragpos);
	vec3 normal = normalize(Normal);
	vec3 lidir = normalize(ldir);
	vec3 ambient = color * 0.1f;
	vec3 diffuse = max(dot(-lidir, normal),0.0f) * color;
	vec3 spec = pow(max(dot(normalize(cdir - lidir), normal), 0.0f), 32) * color;
	Color = vec4(pow(spec + diffuse + ambient, vec3(1.0f / 2.2f)) * (1.0 - shadow), 1.0);
}