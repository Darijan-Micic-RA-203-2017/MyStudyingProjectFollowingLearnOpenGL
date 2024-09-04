#version 330 core

out vec4 FragColor;

// Pass the color of light source and the color of object to the fragment shader.
uniform vec3 colorOfLightSource;
uniform vec3 colorOfObject;

void main()
{
	// Perceived (reflected) color of the object is calculated by doing a component-wise multiplication of
	// light source's color and object's color.
	FragColor = vec4(colorOfLightSource * colorOfObject, 1.0f);
}
