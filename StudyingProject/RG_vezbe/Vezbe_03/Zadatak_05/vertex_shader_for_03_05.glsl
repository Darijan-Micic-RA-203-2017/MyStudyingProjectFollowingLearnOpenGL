#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform float movementOfPointOnXAxis;
uniform float movementOfPointOnYAxis;

void main()
{
	Color = aColor;

	float positionOnXAxis = aPos.x + movementOfPointOnXAxis;
	float positionOnYAxis = aPos.y + movementOfPointOnYAxis;
	gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
}
