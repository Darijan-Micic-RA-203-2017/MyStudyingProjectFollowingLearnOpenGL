#version 330 core

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// Pass the texture object to the fragment shader.
// Each uniform variable of type "sampler2D" is a location of a texture, more commonly known as texture unit.
// The main purpose of texture units is to allow using more than one texture in shaders. Assigning texture
// units to the samplers enables binding to multiple textures at once, as long as the corresponding texture unit
// is activated first.
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
// This uniform is used as "mix" function's third argument.
uniform float mixFactor;

void main()
{
	// Mix the resulting texture color with the vertex color.
	// Mixing colors is achieved by multiplying vectors containing color data.
	// FragColor = texture(ourTexture, TexCoord) * ourColor;

	// Mix the textures assigned to samplers using GLSL's built-in "mix" function.
	// "mix" function takes two values as input and linearly interpolates between them based on its third
	// argument. If the third argument is 0.0f, it returns the first input value. If the third argument is
	// 1.0f, it returns the second input value. A value of 0.2f will return 80 % of the first input color and
	// 20 % of the second input color, resulting in a mixture of both textures.
	FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, vec2(TexCoord.x, TexCoord.y)), 
		mixFactor);
}
