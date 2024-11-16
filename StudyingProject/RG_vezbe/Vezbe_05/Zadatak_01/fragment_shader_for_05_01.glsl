#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixingFactor;

uniform float movementOfTexture0;
uniform float movementOfTexture1;

void main()
{
	vec2 translatedTexCoords = TexCoords;
	translatedTexCoords.s += movementOfTexture0;
	translatedTexCoords.t += movementOfTexture1;
	
	FragColor = mix(texture(texture0, vec2(translatedTexCoords.s, TexCoords.t)), 
		texture(texture1, vec2(TexCoords.s, translatedTexCoords.t)), mixingFactor);
}
