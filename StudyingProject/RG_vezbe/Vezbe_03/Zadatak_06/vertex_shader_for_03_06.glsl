#version 330 core

layout (location = 0) in vec2 aPos;

uniform float movementOfTriangleOnXAxis;
uniform float movementOfTriangleOnYAxis;

void main()
{
	float positionOnXAxis = aPos.x + movementOfTriangleOnXAxis;
	float positionOnYAxis = aPos.y + movementOfTriangleOnYAxis;
	gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
}
