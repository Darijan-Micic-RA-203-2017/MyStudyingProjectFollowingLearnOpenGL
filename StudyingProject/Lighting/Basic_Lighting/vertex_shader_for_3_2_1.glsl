#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

// Pass the transformational matrices to the vertex shader.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	Normal = aNormal;
	
	// Vclip = Mprojection * Mview * Mmodel * Vlocal. Matrix multiplication is meant to be read from right.
	// OpenGL will automatically perform perspective division and clipping after we provide output variable
	// gl_Position with clip space coordinates.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
