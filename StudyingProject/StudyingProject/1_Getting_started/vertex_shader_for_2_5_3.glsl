#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

// out vec4 ourColor;
out vec3 ourPosition;

void main()
{
	// GLSL allows passing vectors as arguments to different vectors constructor calls.
	gl_Position = vec4(aPos, 1.0f);
	// ourColor = aColor;
	ourPosition = aPos;
}
