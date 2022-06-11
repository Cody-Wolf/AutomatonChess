#version 330 core

uniform vec3 color;

out vec4 Color;

in vec4 frag_lpos;
uniform sampler2D dtex;

void main()
{
	vec3 fraglpos = (frag_lpos.xyz / frag_lpos.w) * 0.5 + 0.5;
	float cdepth = fraglpos.z;
	float shadow = 0.0;
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			float ldepth = texture(dtex, vec2(fraglpos.x + i, fraglpos.y)).r;
			shadow += cdepth > ldepth ? 1.0 : 0.0f;
		}
	}
	shadow /= 25.0f;
	Color = vec4(color, 1.0f) * shadow;
}