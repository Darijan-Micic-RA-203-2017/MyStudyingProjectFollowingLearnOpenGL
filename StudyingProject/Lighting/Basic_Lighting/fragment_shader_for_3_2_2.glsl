#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// Pass the position of viewer (needed for specular component of Phong lighting model).
uniform vec3 positionOfViewer;
// Pass the position of light source (needed for diffuse and specular components of Phong lighting model).
uniform vec3 positionOfLightSource;
// Pass the color of light source and the color of object (needed for all 3 components of Phong lighting model).
uniform vec3 colorOfLightSource;
uniform vec3 colorOfObject;

void main()
{
	// Even when it's dark, there's usually still some light somewhere in the world (for example, moon light at
	// large distance), so objects are almost never completely dark. To simulate this scattered light even without
	// a direct light source, we use an ambient lighting constant that always gives the object some color.
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * colorOfLightSource;
	
	vec3 normal = normalize(Normal);
	// The "light's direction". It's a bad name, because we actually need the direction TO light source.
	// The "light's direction" is counted by subtracting fragment's position from the light source's position.
	// Vector visually ends at the minuend (first operand of subtraction) and starts at the subtrahend (second
	// operand of subtraction). Therefore, we want it to end on light source's position, pointing to it.
	vec3 lightDirection = normalize(positionOfLightSource - FragPos);
	// The cosine of angle at which light comes at fragment.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If light emitted from light source comes directly at fragment (at angle of 0 degrees), diffuse factor will
	// be 1 and fragment will be the brightest it can be. The larger the angle at which light comes at fragment
	// is, the less bright that fragment will be. We use "max" function because we do not want the diffuse factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseColor = diffuseFactor * colorOfLightSource;

	// Give the specular highlight a medium-bright color, so that it doesn't impact the color of object too much.
	float specularStrength = 0.5f;
	// The "view direction". It's a bad name, because we actually need the direction TO viewer's position. -||-
	vec3 viewDirection = normalize(positionOfViewer - FragPos);
	// "reflect" function expects the first argument to be a vector pointing from light source to fragment, so we
	// need to negate light direction vector calculated as part of diffuse component.
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// Shininess value of highlight (light source's beam) determines the size of highlight. The higher it is, the
	// light will be more properly reflected, instead of being scattered all around and highlight will be smaller.
	// Shininess of highlight should be a degree of number 2 (2, 4, 8, 16, 32, ...).
	int shininessOfHighlight = 32;
	// The cosine of angle at which reflection comes at viewer.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If reflected light comes directly at viewer (at angle of 0 degrees), specular factor will be 1 and
	// highlight will be the brightest it can be. The larger the angle between light reflection and view direction
	// is, the highlight will be less bright. We use "max" function because we do not want the specular factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), shininessOfHighlight);
	vec3 specularColor = specularStrength * specularFactor * colorOfLightSource;

	// Perceived (reflected) color of the object in Phong lighting model is calculated by doing a component-wise
	// multiplication of: 1) addition of ambient color, diffuse color and specular color; 2) object's color.
	vec3 resultingColorOfFragment = (ambientColor + diffuseColor + specularColor) * colorOfObject;
	FragColor = vec4(resultingColorOfFragment, 1.0f);
}
