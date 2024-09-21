#version 330 core

// "LightSource" structure contains 10 necessary properties of the spotlight.
struct LightSource
{
	// Direction of light (camera's front vector).
	vec3 direction;
	// Position of spotlight in world space (camera's position).
	vec3 position;
	// Cosine of cutoff angle that specifies the radius of the spotlight.
	// If the fragment is inside of the spotlight's radius (cone), it's fully lit.
	// If the fragment is outside of the spotlight's radius (cone), it stays dark.
	float cosOfCutoffAngle;

	// Intensity of the ambient lighting component. It's usually set to a low intensity, because we don't want
	// the ambient color to be too dominant.
	vec3 ambientColor;
	// Intensity of the diffuse lighting component. It's usually set to the exact color we'd like light to have.
	vec3 diffuseColor;
	// Intensity of the specular lighting component. It's usually kept at vec3(1.0f), shining at full intensity.
	vec3 specularColor;

	// Constant parameter of attenuation (Kc) is usually kept at 1.0f. Its main purpose is ensuring that the
	// denominator never gets smaller than 1.0f, which would result in the unwanted effect of raising light's
	// intensity at certain distances.
	float constantParameterOfAttenuation;
	// Linear parameter of attenuation (Kl) is multiplied with the distance between fragment and light source and
	// reduces the intensity of light in a linear fashion.
	float linearParameterOfAttenuation;
	// Quadratic parameter of attenuation (Kq) is multiplied with the quadrant of the distance between fragment
	// and light source and reduces the intensity of light in a quadratic fashion. Quadratic parameter of
	// attenuation will be less significant than linear parameter when the distance is small, but gets much larger
	// as the distance increases.
	float quadraticParameterOfAttenuation;
};

// "Material" structure contains 4 necessary material properties of the surface.
struct Material
{
	// Diffuse map is a texture image that we're indexing for unique color values per fragment. Each fragment of
	// the surface reflects a unique color under diffuse lighting. There's no need for ambient color, as they
	// should always be the same. We indirectly influence ambient color component through diffuse color component.
	sampler2D diffuseMap;
	// Specular map is a texture image that we're indexing for unique color values per fragment. Each fragment of
	// the surface reflects a unique color under specular lighting.
	sampler2D specularMap;
	// Shininess value of highlight (light source's beam) determines the size of highlight. The higher it is, the
	// light will be more properly reflected, instead of being scattered all around and highlight will be smaller.
	// Shininess of highlight should be a degree of number 2 (2, 4, 8, 16, 32, ...).
	float shininessOfHighlight;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Pass the position of viewer (needed for specular component of Phong lighting model).
uniform vec3 positionOfViewer;
// Pass the spotlight.
uniform LightSource lightSource;
// Pass the material of object (needed for all 3 components of Phong lighting model).
uniform Material material;

void main()
{
	vec3 ambientColor = lightSource.ambientColor * vec3(texture(material.diffuseMap, TexCoords));

	// The "light's direction". It's a bad name, because we actually need the direction TO light source.
	// The "light's direction" is counted by subtracting fragment's position from the light source's position.
	// Vector visually ends at the minuend (first operand of subtraction) and starts at the subtrahend (second
	// operand of subtraction). Therefore, we want it to end on light source's position, pointing to it.
	vec3 lightDirection = normalize(lightSource.position - FragPos);
	// The cosine of angle between the "light's direction" and the spotlight's direction (camera's front vector).
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If fragment falls outside of spotlight's radius, calculated cosine will be smaller than cosine of cutoff
	// angle. Cosine function has it's highest values when the angle is smallest. Spotlight's direction (camera's
	// front vector) needs to be negated so that it points towards the light source (the camera itself).
	float cosOfAngleBetweenLightDirAndSpotDir = dot(lightDirection, normalize(-lightSource.direction));
	if (cosOfAngleBetweenLightDirAndSpotDir < lightSource.cosOfCutoffAngle)
	{
		// Perceived (reflected) color of the object is only the object's ambient color component, since it's
		// outside of spotlight's radius.
		vec3 resultingColorOfFragment = ambientColor;
		FragColor = vec4(ambientColor, 1.0f);

		return;
	}

	vec3 normal = normalize(Normal);
	// The cosine of angle at which light comes at fragment.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If light emitted from light source comes directly at fragment (at angle of 0 degrees), diffuse factor will
	// be 1 and fragment will be the brightest it can be. The larger the angle at which light comes at fragment
	// is, the less bright that fragment will be. We use "max" function because we do not want the diffuse factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseColor = lightSource.diffuseColor * (diffuseFactor * vec3(texture(material.diffuseMap, TexCoords)));

	// The "view direction". It's a bad name, because we actually need the direction TO viewer's position. -||-
	vec3 viewDirection = normalize(positionOfViewer - FragPos);
	// "reflect" function expects the first argument to be a vector pointing from light source to fragment, so we
	// need to negate light direction vector calculated as part of diffuse component.
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// The cosine of angle at which reflection comes at viewer.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If reflected light comes directly at viewer (at angle of 0 degrees), specular factor will be 1 and
	// highlight will be the brightest it can be. The larger the angle between light reflection and view direction
	// is, the highlight will be less bright. We use "max" function because we do not want the specular factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininessOfHighlight);
	vec3 specularColor = lightSource.specularColor * 
		(specularFactor * vec3(texture(material.specularMap, TexCoords)));

	// Calculate distance between fragment and light source using GLSL's built-in "length" function.
	float d = length(lightSource.position - FragPos);
	// Calculate attenuation factor "Fatt". Attenuation factor is the measure of light's leftover intensity at a
	// given distance "d" between fragment and light source. Higher the distance, the more fading out will happen.
	// Fatt = 1.0f / (Kc + Kl * d + Kq * d^2).
	float attenuationFactor = 1.0f / (lightSource.constantParameterOfAttenuation + 
		lightSource.linearParameterOfAttenuation * d + lightSource.quadraticParameterOfAttenuation * pow(d, 2.0f));
	// Use attenuation factor only on diffuse and specular Phong lighting model's components of fragment's color.
	// Ambient component shouldn't be attenuated when using a spotlight. Doing so would result in light having a
	// lower intensity inside of spotlight's radius than outside of it at greater distances.
	diffuseColor *= attenuationFactor;
	specularColor *= attenuationFactor;

	// Perceived (reflected) color of the object in Phong lighting model is calculated by doing an addition of
	// ambient color, diffuse color and specular color.
	vec3 resultingColorOfFragment = ambientColor + diffuseColor + specularColor;
	FragColor = vec4(resultingColorOfFragment, 1.0f);
}
