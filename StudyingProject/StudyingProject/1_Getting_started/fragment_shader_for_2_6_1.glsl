#version 330 core

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// Pass the texture object to the fragment shader.
uniform sampler2D ourTexture;

void main()
{
	// Mix the resulting texture color with the vertex colors.
	// Mixing colors is achieved by multiplying vectors containing color data.
	FragColor = texture(ourTexture, TexCoord) * ourColor;
}
