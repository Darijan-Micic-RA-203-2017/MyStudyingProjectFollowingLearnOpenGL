#version 330 core

out vec4 FragColor;

uniform int windowWidth;
uniform int windowHeight;

void main()
{
	vec3 colorOfTriangle = vec3(1.0f);
	// 1. quadrant: paint the fragment in red.
	if (gl_FragCoord.x > windowWidth / 2 && gl_FragCoord.y > windowHeight / 2)
	{
		colorOfTriangle = vec3(1.0f, 0.0f, 0.0f);
	}
	// 3. quadrant: paint the fragment in blue.
	if (gl_FragCoord.x < windowWidth / 2 && gl_FragCoord.y < windowHeight / 2)
	{
		colorOfTriangle = vec3(0.0f, 0.0f, 1.0f);
	}
	// 4. quadrant: paint the fragment in purple.
	if (gl_FragCoord.x > windowWidth / 2 && gl_FragCoord.y < windowHeight / 2)
	{
		colorOfTriangle = vec3(1.0f, 0.0f, 1.0f);
	}

	FragColor = vec4(colorOfTriangle, 1.0f);
}
