#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

// Pass the normal matrix to the vertex shader.
uniform mat3 normalMatrix;
// Pass the transformational matrices to the vertex shader.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	// We're going to do all lighting calculations in world space, so fragment's position needs to be in world
	// coordinates. To convert vertex's position to world coordinates, we multiply it with the model matrix only.
	// Constructor that takes a parameter of type "vec4" of class "vec3" discards the last row.
	FragPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	// Multiply the normal vector with the normal matrix to ensure that normal vector is still perpendicular to
	// vertex's surface. In other words, all transformations done in the model matrix are undone.
	Normal = normalMatrix * aNormal;
	
	// Vclip = Mprojection * Mview * Mmodel * Vlocal. Matrix multiplication is meant to be read from right.
	// OpenGL will automatically perform perspective division and clipping after we provide output variable
	// gl_Position with clip space coordinates.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
