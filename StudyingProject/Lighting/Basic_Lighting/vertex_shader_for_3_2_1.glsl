#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

// Pass the transformational matrices to the vertex shader.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	// We're going to do all lighting calculations in world space, so fragment's position needs to be in world
	// coordinates. To convert vertex's position to world coordinates, we multiply it with the model matrix only.
	FragPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	// Normal matrix is a model matrix specifically tailored for normal vectors. Normal matrix is defined as
	// the transpose of the inverse of the upper-left 3x3 part of the model matrix.
	// Non-uniform scaling would transform vertex in such a way that the normal vector would no longer be
	// perpendicular to the vertex's surface. This means that the lighting of surface would be distorted. We
	// mitigate non-uniform scaling by multiplying normal vector with normal matrix.
	// NOTE: Inverting a matrix is a costly operation for shaders, so it should actually be calculated on CPU and
	// sent to shader program via a uniform.
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	
	// Vclip = Mprojection * Mview * Mmodel * Vlocal. Matrix multiplication is meant to be read from right.
	// OpenGL will automatically perform perspective division and clipping after we provide output variable
	// gl_Position with clip space coordinates.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
