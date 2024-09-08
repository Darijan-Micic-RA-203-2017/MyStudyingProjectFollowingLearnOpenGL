#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// WHEN THE PHONG LIGHTING MODEL IS IMPLEMENTED IN THE VERTEX SHADER, IT'S CALLED THE GOURAUD SHADING.
// Note that, due to the interpolation, the lighting looks somewhat off. The advantage of doing lighting in the
// vertex shader is that it's a lot more efficient, since there are usually a lot less vertices compared to
// fragments. Expensive lighting calculations are done less frequently. However, the resulting color value in the
// vertex shader is the resulting lighting color of only that vertex. The color values of the surrounding
// fragments are then the result of interpolated lighting colors. LIGHTING IS NOT VERY REALISTIC UNLESS LARGE
// AMOUNTS OF VERTICES ARE USED, WHICH DEFEATS THE PURPOSE OF IMPLEMENTING GOURAUD SHADING.
out vec3 ResultingColorOfLightSource;

// Pass the color of light source (needed for all 3 components of Phong lighting model).
uniform vec3 colorOfLightSource;
// Pass the position of light source (needed for diffuse and specular components of Phong lighting model).
uniform vec3 positionOfLightSource;
// Pass the normal matrix to the vertex shader.
uniform mat3 normalMatrix;
// Pass the transformational matrices to the vertex shader.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	// We're going to do all lighting calculations in view space, so fragment's position needs to be in view
	// coordinates. To convert vertex's position to view coordinates, we multiply it with the model matrix and
	// the view matrix. Constructor that takes a parameter of type "vec4" of class "vec3" discards the last row.
	vec3 fragPos = vec3(viewMatrix * modelMatrix * vec4(aPos, 1.0f));
	// Multiply the normal vector with the normal matrix to ensure that normal vector is still perpendicular to
	// vertex's surface. In other words, all transformations in the model matrix and the view matrix are undone.
	vec3 normal = normalize(normalMatrix * aNormal);
	// Convert the position of light source from world space to view space.
	vec3 lightPos = vec3(viewMatrix * vec4(positionOfLightSource, 1.0f));

	// Even when it's dark, there's usually still some light somewhere in the world (for example, moon light at
	// large distance), so objects are almost never completely dark. To simulate this scattered light even without
	// a direct light source, we use an ambient lighting constant that always gives the object some color.
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * colorOfLightSource;
	
	// The "light's direction". It's a bad name, because we actually need the direction TO light source.
	// The "light's direction" is counted by subtracting fragment's position from the light source's position.
	// Vector visually ends at the minuend (first operand of subtraction) and starts at the subtrahend (second
	// operand of subtraction). Therefore, we want it to end on light source's position, pointing to it.
	vec3 lightDirection = normalize(lightPos - fragPos);
	// The cosine of angle at which light comes at fragment.
	// For      vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle).
	// For unit vectors v and w: dot(v, w) = ||v|| * ||w|| * cos(angle) = 1 * 1 * cos(angle) = cos(angle).
	// If light emitted from light source comes directly at fragment (at angle of 0 degrees), diffuse factor will
	// be 1 and fragment will be the brightest it can be. The larger the angle at which light comes at fragment
	// is, the less bright that fragment will be. We use "max" function because we do not want the diffuse factor
	// to be negative. Lighting for negative colors is not well defined and we avoid working with negative colors.
	float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseColor = diffuseFactor * colorOfLightSource;

	// Specular highlight should be given a medium-bright color, so that it doesn't impact the color of object
	// too much. In this example, it is maximized to better show the effect of Gouraud shading.
	float specularStrength = 1.0f;
	// The "view direction". It's a bad name, because we actually need the direction TO viewer's position. -||-
	// Position of viewer is always (0.0f, 0.0f, 0.0f) in view space.
	vec3 viewDirection = normalize(-fragPos);
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

	// Resulting color of the light source in Phong lighting model is calculated by doing an addition of
	// ambient color, diffuse color and specular color.
	ResultingColorOfLightSource = ambientColor + diffuseColor + specularColor;
	
	// Vclip = Mprojection * Mview * Mmodel * Vlocal. Matrix multiplication is meant to be read from right.
	// OpenGL will automatically perform perspective division and clipping after we provide output variable
	// gl_Position with clip space coordinates.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
