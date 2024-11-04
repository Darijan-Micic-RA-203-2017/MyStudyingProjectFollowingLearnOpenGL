#version 330 core

in vec3 Color;
in vec3 ColorOffsetBasedOnDistanceFromXAxis;

out vec4 FragColor;

void main()
{
	FragColor = vec4(Color + ColorOffsetBasedOnDistanceFromXAxis, 1.0f);
}
