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

// "Material" structure contains 4 necessary material properties of the surface.
struct Material
{
	// Color the surface reflects under ambient lighting.
	vec3 ambientColor;
	// Color the surface reflects under diffuse lighting.
	vec3 diffuseColor;
	// Color the surface reflects under specular lighting.
	vec3 specularColor;
	// Shininess value of highlight (light source's beam) determines the size of highlight. The higher it is, the
	// light will be more properly reflected, instead of being scattered all around and highlight will be smaller.
	// Shininess of highlight should be a degree of number 2 (2, 4, 8, 16, 32, ...).
	float shininessOfHighlight;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// Pass the position of viewer (needed for specular component of Phong lighting model).
uniform vec3 positionOfViewer;
// Pass the light source (needed for all 3 components of Phong lighting model).
uniform LightSource lightSource;
// Pass the material of object (needed for all 3 components of Phong lighting model).
uniform Material material;

void main()
{
	vec3 ambientColor = lightSource.ambientColor * material.ambientColor;
	
	vec3 normal = normalize(Normal);
	// The "light's direction". It's a bad name, because we actually need the direction TO light source.
	// The "light's direction" is counted by subtracting fragment's position from the light source's position.
	// Vector visually ends at the minuend (first operand of subtraction) and starts at the subtrahend (second
	// operand of subtraction). Therefore, we want it to end on light source's position, pointing to it.
	vec3 lightDirection = normalize(lightSource.position - FragPos);
	// The cosine of angle at which light comes at fragment.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If light emitted from light source comes directly at fragment (at angle of 0 degrees), diffuse factor will
	// be 1 and fragment will be the brightest it can be. The larger the angle at which light comes at fragment
	// is, the less bright that fragment will be. We use "max" function because we do not want the diffuse factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseColor = lightSource.diffuseColor * (diffuseFactor * material.diffuseColor);

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
	vec3 specularColor = lightSource.specularColor * (specularFactor * material.specularColor);

	// Perceived (reflected) color of the object in Phong lighting model is calculated by doing an addition of
	// ambient color, diffuse color and specular color.
	vec3 resultingColorOfFragment = ambientColor + diffuseColor + specularColor;
	FragColor = vec4(resultingColorOfFragment, 1.0f);
}
