#extern version.glsl

in vec2 uv1;
in vec2 uv2;
in float blendf;
in float fade;

out vec4 vertexColor;

uniform sampler2D texture_sampler;
uniform vec4 color;

void main(void)
{
	vec4 tex1 = texture(texture_sampler, uv1);
	vec4 tex2 = texture(texture_sampler, uv2);

	vec4 tex = mix(tex1, tex2, blendf);
	tex.w *= fade;
	
	vertexColor = tex * color;
}
