#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform bool marbleTextureNeeded;
uniform sampler2D marbleTexture;
uniform sampler2D metalTexture;
uniform float depthOfNearPlane;
uniform float depthOfFarPlane;

float linearizeDepthInScreenSpace(float zValueOfFragment);

void main()
{
	/*
	if (marbleTextureNeeded)
	{
		FragColor = texture(marbleTexture, TexCoords);
	}
	else
	{
		FragColor = texture(metalTexture, TexCoords);
	}
	*/

	// Depth testing is done in screen space after the fragment shader has run. The screen space coordinates relate
	// directly to the viewport defined by OpenGL's "glViewport" function. These coordinates can be directly
	// accessed here (in the fragment shader) by GLSL's built-in "gl_FragCoord" variable. This variable's
	// x-component and y-component are fragment's x-coordinate and y-coordinate in screen space, with (0.0f, 0.0f)
	// being the bottom left corner. Its z-component is the depth value of the fragment in screen space.
	float depthInViewSpace = linearizeDepthInScreenSpace(gl_FragCoord.z);
	// Linearized depth values are in range [depth of near plane, depth of far plane], which means that the
	// majority of them will be higher than 1.0f. We don't want the majority of the scene to be white, so we'll
	// divide linearized depth with depth of far plane to convert it to range [0.0f, 1.0f] (range of color values).
	float depthInViewSpaceAsColor = depthInViewSpace / depthOfFarPlane;
	FragColor = vec4(vec3(depthInViewSpaceAsColor), 1.0f);
}

// Transform the non-linear depth value of the fragment to its linear sibling. This basically means that we need to
// inverse the process done in the projection matrix. In other words, we need to convert depth values (in range
// [0.0f, 1.0f]) from screen space back to view space (in range [near plane depth, far plane depth]).
float linearizeDepthInScreenSpace(float zValueOfFragment)
{
	// 1. step: Convert depth value to a normalized device coordinate (NDC).
	float zValueAsNDC = 2.0f * zValueOfFragment - 1.0f;
	// 2. step: Convert NDC to a depth value in view space.
	float linearizedDepth = (2.0f * depthOfNearPlane * depthOfFarPlane) / 
		(depthOfNearPlane + depthOfFarPlane - zValueAsNDC * (depthOfFarPlane - depthOfNearPlane));

	return linearizedDepth;
}
