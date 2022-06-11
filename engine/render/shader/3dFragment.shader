#version 330 core

uniform vec3 color;

out vec4 Color;

in vec4 frag_lpos;
uniform sampler2D dtex;

void main()
{
	vec3 fraglpos = (frag_lpos.xyz / frag_lpos.w) * 0.5 + 0.5;
	float ldepth = texture(dtex, fraglpos.xy).r;
	float cdepth = fraglpos.z;
	float shadow = cdepth > ldepth ? 1.0 : 0.0f;
	Color = vec4(color, 1.0f) * shadow;
}