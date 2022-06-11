#version 330 core

uniform sampler2D tex;

in vec2 tcoord;
out vec4 Color;

in vec4 frag_lpos;
uniform sampler2D dtex;

void main()
{
	vec3 fraglpos = (frag_lpos.xyz / frag_lpos.w) * 0.5 + 0.5;
	float ldepth = texture(dtex, fraglpos.xy).r;
	float bias = 0.005;
	float cdepth = fraglpos.z;
	float shadow = cdepth - bias > ldepth ? 1.0 : 0.0f;
	Color = texture(tex, tcoord) * (1.0 - shadow);
}