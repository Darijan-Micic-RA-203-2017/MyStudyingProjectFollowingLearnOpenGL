#version 330 core

out vec4 FragColor;

uniform float greenColorAmount;

void main()
{
	// We get the yellow color by combining the red and the green color.
	FragColor = vec4(1.0f, greenColorAmount, 0.0f, 1.0f);
}
