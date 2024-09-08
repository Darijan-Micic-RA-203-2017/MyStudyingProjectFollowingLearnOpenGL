#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightSourcePos;

// Pass the position of light source (needed for diffuse and specular components of Phong lighting model).
uniform vec3 positionOfLightSource;
// Pass the normal matrix to the vertex shader.
uniform mat3 normalMatrix;
// Pass the transformational matrices to the vertex shader.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	// We're going to do all lighting calculations in view space, so fragment's position needs to be in view
	// coordinates. To convert vertex's position to view coordinates, we multiply it with the model matrix and
	// the view matrix. Constructor that takes a parameter of type "vec4" of class "vec3" discards the last row.
	FragPos = vec3(viewMatrix * modelMatrix * vec4(aPos, 1.0f));
	// Multiply the normal vector with the normal matrix to ensure that normal vector is still perpendicular to
	// vertex's surface. In other words, all transformations in the model matrix and the view matrix are undone.
	Normal = normalMatrix * aNormal;
	// Convert the position of light source from world space to view space.
	LightSourcePos = vec3(viewMatrix * vec4(positionOfLightSource, 1.0f));
	
	// Vclip = Mprojection * Mview * Mmodel * Vlocal. Matrix multiplication is meant to be read from right.
	// OpenGL will automatically perform perspective division and clipping after we provide output variable
	// gl_Position with clip space coordinates.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
