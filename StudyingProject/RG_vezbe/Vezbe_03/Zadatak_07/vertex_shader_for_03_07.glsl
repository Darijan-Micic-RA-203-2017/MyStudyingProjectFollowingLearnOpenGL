#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;
out vec3 ColorOffsetBasedOnDistanceFromXAxis;

uniform float movementOfTrianglesOnXAxis;
uniform float movementOfTrianglesOnYAxis;

void main()
{
	Color = aColor;

	// Through the trial-and-error method, I found out the calculated movement of the triangles on the x-axis has to be
	// subtracted from the fragment's x-coordinate in order to make the upper triangle move clockwise and the lower triangle
	// to move anti-clockwise. Addition would result in the opposite outcome.
	float positionOnXAxis = aPos.x - movementOfTrianglesOnXAxis;
	float positionOnYAxis = aPos.y;
	// Upper triangle (above the x-axis): move clockwise.
	if (aPos.y > 0.0f)
	{
		positionOnYAxis += movementOfTrianglesOnYAxis;
		// Subtract 0.5f from the color offset to darken the triangle when it's close to the x-axis, or to brighten the
		// triangle when it's far from the x-axis.
		ColorOffsetBasedOnDistanceFromXAxis = vec3(positionOnYAxis - 0.5f);

		gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
	}
	// Lower triangle (below the x-axis): move anti-clockwise.
	else
	{
		positionOnYAxis -= movementOfTrianglesOnYAxis;
		// Subtract 0.5f from the color offset to darken the triangle when it's close to the x-axis, or to brighten the
		// triangle when it's far from the x-axis.
		ColorOffsetBasedOnDistanceFromXAxis = vec3(abs(positionOnYAxis) - 0.5f);

		gl_Position = vec4(positionOnXAxis, positionOnYAxis, 0.0f, 1.0f);
	}
}
