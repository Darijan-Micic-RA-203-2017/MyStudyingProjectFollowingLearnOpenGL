#version 330 core

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// Pass the texture object to the fragment shader.
uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}
