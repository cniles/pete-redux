#version 130

uniform sampler2D texture0;
uniform vec4 color;
in vec2 texCoord0;

out vec4 outColor;

void main(void)
{
	outColor = texture(texture0, texCoord0.st);
	if(outColor.a < 0.1)
	{
		discard;
	}
	outColor = outColor * color;
}
