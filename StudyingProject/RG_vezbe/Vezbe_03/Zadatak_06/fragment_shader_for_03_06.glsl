#version 330 core

out vec4 FragColor;

uniform int windowWidth;
uniform int windowHeight;

void main()
{
	vec3 colorOfTriangle = vec3(1.0f);
	// 1. quadrant: paint the fragment with red color.
	if (gl_FragCoord.x > windowWidth / 2 && gl_FragCoord.y > windowHeight / 2)
	{
		colorOfTriangle = vec3(1.0f, 0.0f, 0.0f);
	}
	// Bottom half of the screen: paint as vertical stripes (I chose for those stripes to be painted with yellow color).
	else if (gl_FragCoord.y < windowHeight / 2)
	{
		// 3. quadrant: paint the fragment with blue color.
		if (gl_FragCoord.x < windowWidth / 2)
		{
			if (mod(gl_FragCoord.x, 15) < 7)
			{
				colorOfTriangle = vec3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				colorOfTriangle = vec3(1.0f, 1.0f, 0.0f);
			}
		}
		// 4. quadrant: paint the fragment with purple color.
		else
		{
			if (mod(gl_FragCoord.x, 15) < 7)
			{
				colorOfTriangle = vec3(1.0f, 0.0f, 1.0f);
			}
			else
			{
				colorOfTriangle = vec3(1.0f, 1.0f, 0.0f);
			}
		}
	}

	FragColor = vec4(colorOfTriangle, 1.0f);
}
