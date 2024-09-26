#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

// Pass the texture object to the fragment shader.
// Each uniform variable of type "sampler2D" is a location of a texture, more commonly known as texture unit.
// The main purpose of texture units is to allow using more than one texture in shaders. Assigning texture
// units to the samplers enables binding to multiple textures at once, as long as the corresponding texture unit
// is activated first.
uniform sampler2D diffuseMap;

void main()
{
	// We'll only use the diffuse map, no lighting for now.
	FragColor = texture(diffuseMap, TexCoords);
}
