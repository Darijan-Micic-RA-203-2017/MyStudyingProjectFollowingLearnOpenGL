#version 330 core

in vec3 ResultingColorOfLightSource;

out vec4 FragColor;

// Pass the color of object (needed for all 3 components of Phong lighting model).
uniform vec3 colorOfObject;

void main()
{
	// Perceived (reflected) color of the object in Phong lighting model is calculated by doing a component-wise
	// multiplication of: 1) addition of ambient color, diffuse color and specular color; 2) object's color.
	FragColor = vec4(ResultingColorOfLightSource * colorOfObject, 1.0f);
}
