#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform bool marbleTextureNeeded;
uniform sampler2D marbleTexture;
uniform sampler2D metalTexture;

void main()
{
	if (marbleTextureNeeded)
	{
		FragColor = texture(marbleTexture, TexCoords);
	}
	else
	{
		FragColor = texture(metalTexture, TexCoords);
	}
}
