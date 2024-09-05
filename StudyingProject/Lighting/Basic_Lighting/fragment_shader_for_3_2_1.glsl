#version 330 core

out vec4 FragColor;

// Pass the color of light source and the color of object to the fragment shader.
uniform vec3 colorOfLightSource;
uniform vec3 colorOfObject;

void main()
{
	// Even when it's dark, there's usually still some light somewhere in the world (for example, moon light at
	// large distance), so objects are almost never completely dark. To simulate this scattered light even without
	// a direct light source, we use an ambient lighting constant that always gives the object some color.
	float ambientFactor = 0.1f;
	vec3 colorOfAmbient = ambientFactor * colorOfLightSource;

	// Perceived (reflected) color of the object is calculated by doing a component-wise multiplication of
	// ambient's color (scaled light source's color) and object's color.
	vec3 resultingColorOfFragment = colorOfAmbient * colorOfObject;
	FragColor = vec4(resultingColorOfFragment, 1.0f);
}
