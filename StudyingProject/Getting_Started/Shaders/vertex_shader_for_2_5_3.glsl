#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec3 ourPosition;
// out vec4 ourColor;

void main()
{
	ourPosition = aPos;
	// ourColor = aColor;

	// GLSL allows passing vectors as arguments to different vectors constructor calls.
	gl_Position = vec4(aPos, 1.0f);
}
