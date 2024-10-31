#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 Color;

uniform float movementOfSquareOnXAxis;
uniform float movementOfSquareOnYAxis;

void main()
{
	Color = aColor;

	float positionOnXAxis = aPos.x + movementOfSquareOnXAxis;
	float positionOnYAxis = aPos.y + movementOfSquareOnYAxis;

	gl_Position = vec4(positionOnXAxis, positionOnYAxis, aPos.z, 1.0f);
}
