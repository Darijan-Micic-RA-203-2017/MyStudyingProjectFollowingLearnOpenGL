#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform float movementOfTrianglesOnXAxis;
uniform float movementOfTrianglesOnYAxis;

void main()
{
	Color = aColor;

	// Upper triangle (above the x-axis): move clockwise.
	if (aPos.y > 0.0f)
	{
		float positionOnXAxis = aPos.x - movementOfTrianglesOnXAxis;
		float positionOnYAxis = aPos.y + movementOfTrianglesOnYAxis;
		gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
	}
	// Lower triangle (below the x-axis): move anti-clockwise.
	else
	{
		float positionOnXAxis = aPos.x - movementOfTrianglesOnXAxis;
		float positionOnYAxis = aPos.y - movementOfTrianglesOnYAxis;
		gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
	}
}
