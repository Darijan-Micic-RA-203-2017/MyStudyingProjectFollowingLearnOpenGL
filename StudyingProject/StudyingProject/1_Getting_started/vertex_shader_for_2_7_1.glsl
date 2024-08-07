#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

// Transformational matrix that we will multiply with position vector.
uniform mat4 transformationalMatrix;

void main()
{
	gl_Position = transformationalMatrix * vec4(aPos, 1.0f);
	ourColor = aColor;
	TexCoord = aTexCoord;
}
