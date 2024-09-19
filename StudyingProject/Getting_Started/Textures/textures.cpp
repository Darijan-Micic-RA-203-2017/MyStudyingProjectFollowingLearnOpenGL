#include "textures.h"

const int window_width = 800;
const int window_height = 600;

int draw_textures()
{
	// Initialize the GLFW library.
	if (!glfwInit())
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	// Specify the OpenGL version and profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window and make the context of created window the main context on the current thread.
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, 
		"Getting Started - Textures", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_textures);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Compile our shaders and link our shader program using helper class.
	ShaderProgram ourShaderProgram("Textures/vertex_shader_for_2_6_1.glsl", 
		"Textures/fragment_shader_for_2_6_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}
	
	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// First three values represent position of vertex, middle four values represent color of vertex, while
	// last two values represent texture coordinates (from 0.0f to 1.0f).
	float vertices[] = {
		// We intentionally changed texture coordinates to zoom in on texture image.
		// position         // color                // texture coordinates
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.45f, 0.45f, // bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.55f, 0.45f, // bottom right
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.55f, 0.55f, // top right
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.45f, 0.55f  // top left
	};
	// Indices, which start at 0.
	unsigned int indices[] = {
		0u, 1u, 3u, 
		1u, 2u, 3u
	};

	// Create memory on the GPU where vertex data and index data will be stored.
	// Said data will be handled by VAO and vertex/element buffer objects inside that VAO.
	// Core OpenGL REQUIRES the use of VAOs!
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Bind (assign) the newly created VAO to OpenGL's context.
	glBindVertexArray(VAO);

	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Bind (assign) the newly created EBO to OpenGL's context.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Copy user-defined data into the currently bound buffer.
	// Index data is now stored on the graphics card's memory.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	// Position attribute.
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);
	// Color attribute.
	glVertexAttribPointer(1u, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex color attribute.
	glEnableVertexAttribArray(1u);
	// Texture coordinate attribute.
	glVertexAttribPointer(2u, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (7 * sizeof(float)));
	// Enable vertex texture coordinate attribute.
	glEnableVertexAttribArray(2u);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	// Create memory on the GPU where texture will be stored.
	unsigned int texture;
	glGenTextures(1, &texture);
	// Bind (assign) the newly created texture to OpenGL's context.
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set texture wrapping parameters. Texture coordinates are in range [0.0f, 1.0f]. If texture coordinates
	// are specified outside of mentioned range, texture wrapping option determines the look.
	// Each texture wrapping option can be set per coordinate axis (s, t and r if 3D textures are used).
	// s-axis, t-axis and r-axis correspond to x-axis, y-axis and z-axis, respectively.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// We intentionally set texture wrapping to "GL_CLAMP_TO_EDGE".

	// Set texture filtering parameters. Texture coordinates do not depend on resolution, but can be any
	// floating point value. Therefore, OpenGL needs to figure out which texture pixel (texel) to map the
	// texture coordinate to. Nearest neighbour filtering is better suited for minifying operations,
	// while (bi)linear filtering is better suited for magnifying operations.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Mipmaps are used to improve minifying, not magnifying. Setting one of the mipmap filtering options as
	// the magnification filter will generate the OpenGL "GL_INVALID_ENUM" error code.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// We intentionally set texture filtering to nearest neighbour filtering, so that individual texels
	// could be seen.

	// Load the image that will be used as a texture.
	int textureImageWidth;
	int textureImageHeight;
	int numberOfColorChannelsInTextureImage;
	unsigned char* pixels = stbi_load("resources/wooden_container.jpg", &textureImageWidth, &textureImageHeight, 
		&numberOfColorChannelsInTextureImage, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImageWidth, textureImageHeight, 0, GL_RGB, 
			GL_UNSIGNED_BYTE, pixels);
		// Automatically generate all the required mipmaps for the currently bound texture.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Image of would-be-texture could not be loaded!" << std::endl;
		stbi_image_free(pixels);
		glfwTerminate();

		return 7;
	}
	// Free the image memory.
	stbi_image_free(pixels);

	// Unbind texture for safety reasons. This is not neccessary.
	glBindTexture(GL_TEXTURE_2D, 0u);
	
	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// First part: Process the user's input.
		processInput_for_textures(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader program.
		// Every shader and rendering call from now on will use this shader program object.
		ourShaderProgram.useProgram();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0u);

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback function.
void framebuffer_size_callback_for_textures(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input processing function.
void processInput_for_textures(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
