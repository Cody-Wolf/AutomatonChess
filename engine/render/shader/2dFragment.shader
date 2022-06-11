#version 330 core

uniform sampler2D tex;

in vec2 tcoord;
out vec4 Color;

in vec4 frag_lpos;
uniform sampler2D dtex;

void main()
{
	vec3 fraglpos = (frag_lpos.xyz / frag_lpos.w) * 0.5 + 0.5;
	float cdepth = fraglpos.z;
	float shadow = 0.0;
	float bias = 0.005;
	vec2 texelSize = 1.0 / textureSize(dtex, 0);
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			float ldepth = texture(dtex, fraglpos.xy + vec2(i, j) * texelSize).r;
			shadow += cdepth - bias > ldepth ? 1.0 : 0.0f;
		}
	}
	shadow /= 9.0f;
	Color = texture(tex, tcoord) * (1.0 - shadow);
}