#version 330 core

// "Material" structure contains all necessary material properties of the surface.
// Each uniform variable of type "sampler2D" is a location of a texture, more commonly known as texture unit.
// The main purpose of texture units is to allow using more than one texture in shaders. Assigning texture
// units to the samplers enables binding to multiple textures at once, as long as the corresponding texture unit
// is activated first.
struct Material
{
	// Diffuse map is a texture image that we're indexing for unique color values per fragment. Each fragment of
	// the surface reflects a unique color under diffuse lighting. There's no need for ambient color, as they
	// should always be the same. We indirectly influence ambient color component through diffuse color component.
	sampler2D texture_diffuse1;
};

in vec2 TexCoords;

out vec4 FragColor;

// Pass the material of object (needed for all 3 components of Phong lighting model).
uniform Material material;

void main()
{
	// We'll only use the diffuse map, no lighting for now.
	FragColor = texture(material.texture_diffuse1, TexCoords);
}
