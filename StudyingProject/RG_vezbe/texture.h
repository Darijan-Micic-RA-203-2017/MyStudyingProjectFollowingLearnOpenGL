// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the texture header. This prevents linking conflicts.
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include "stb_image.h"

class Texture
{
public:
	unsigned int id = 0u;
	int errorCode = 0;

	Texture(const char* imagePath)
	{
		// Create memory on the GPU where texture will be stored.
		glGenTextures(1, &id);
		// Bind (assign) the newly created texture to OpenGL's context.
		glBindTexture(GL_TEXTURE_2D, id);

		// Set texture wrapping parameters. Texture coordinates are in range [0.0f, 1.0f]. If texture coordinates
		// are specified outside of mentioned range, texture wrapping option determines the look.
		// Each texture wrapping option can be set per coordinate axis (s, t and r if 3D textures are used).
		// s-axis, t-axis and r-axis correspond to x-axis, y-axis and z-axis, respectively.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture filtering parameters. Texture coordinates do not depend on resolution, but can be any
		// floating point value. Therefore, OpenGL needs to figure out which texture pixel (texel) to map the
		// texture coordinate to. Nearest neighbour filtering is better suited for minifying operations, while
		// (bi)linear filtering is better suited for magnifying operations.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Mipmaps are used to improve minifying, not magnifying. Setting one of the mipmap filtering options as
		// the magnification filter will generate the OpenGL "GL_INVALID_ENUM" error code.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Tell "stb_image.h" library to flip the y-axis during image loading. This call is necessary because
		// OpenGL expects the 0.0f coordinate on the y-axis to be on the bottom side of the image, but images
		// usually have 0.0f at the top of the y-axis.
		stbi_set_flip_vertically_on_load(true);

		// Load the image that will be used as a texture.
		int textureImageWidth, textureImageHeight, numberOfColorChannelsInTextureImage;
		unsigned char* pixels = stbi_load(imagePath, &textureImageWidth, &textureImageHeight, 
			&numberOfColorChannelsInTextureImage, 0);
		
		if (pixels)
		{
			// Determine image format from number of color channels in texture image.
			GLenum format = 0u;
			switch (numberOfColorChannelsInTextureImage)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				stbi_image_free(pixels);
				errorCode = 8;

				return;
			}

			// Generate a texture using the previously loaded image data (pixels).
			glTexImage2D(GL_TEXTURE_2D, 0, format, textureImageWidth, textureImageHeight, 0, format, 
				GL_UNSIGNED_BYTE, pixels);
			// Automatically generate all the required mipmaps for the currently bound texture.
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Image of would-be-texture could not be loaded!" << std::endl;
			errorCode = 7;
		}
		// Free the image memory.
		stbi_image_free(pixels);
	}
};
#endif
