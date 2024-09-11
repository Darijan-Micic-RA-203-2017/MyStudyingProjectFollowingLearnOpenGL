#version 330 core

// "LightSource" structure contains 4 necessary properties of the light source.
struct LightSource
{
	// Position of light source in world space.
	vec3 position;
	// Intensity of the ambient lighting component. It's usually set to a low intensity, because we don't want
	// the ambient color to be too dominant.
	vec3 ambientColor;
	// Intensity of the diffuse lighting component. It's usually set to the exact color we'd like light to have.
	vec3 diffuseColor;
	// Intensity of the specular lighting component. It's usually kept at vec3(1.0f), shining at full intensity.
	vec3 specularColor;
};

out vec4 FragColor;

// Pass the light source (needed for all 3 components of Phong lighting model).
uniform LightSource lightSource;

void main()
{
	// Perceived color of the object in Phong lighting model is calculated by doing an addition of ambient color,
	// diffuse color and specular color.
	vec3 resultingColorOfLightSource = lightSource.ambientColor + lightSource.diffuseColor + lightSource.specularColor;
	// Tell light source to take the color of light it's emitting.
	FragColor = vec4(resultingColorOfLightSource, 1.0f);
}
