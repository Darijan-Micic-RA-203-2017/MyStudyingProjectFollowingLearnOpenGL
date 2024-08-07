#include "transformations.h"

const int window_width = 800;
const int window_height = 600;

float currentMixFactor = 0.2f;

int draw_transformations()
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
		"StudyingProject - Transformations", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_transformations);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	Shader ourShaderProgram("1_Getting_started/vertex_shader_for_2_7_1.glsl",
		"1_Getting_started/fragment_shader_for_2_7_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// First three values represent position of vertex, middle four values represent color of vertex, while
	// last two values represent texture coordinates (from 0.0f to 1.0f).
	float vertices[] = {
		// position         // color                // texture coordinates
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f  // top left
	};
	// Indices, which start at 0.
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0);
	// Color attribute.
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex color attribute.
	glEnableVertexAttribArray(1);
	// Texture coordinate attribute.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (7 * sizeof(float)));
	// Enable vertex texture coordinate attribute.
	glEnableVertexAttribArray(2);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create memory on the GPU where first texture will be stored.
	unsigned int texture1;
	glGenTextures(1, &texture1);
	// Bind (assign) the newly created texture to OpenGL's context.
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set texture wrapping parameters. Texture coordinates are in range [0.0f, 1.0f]. If texture coordinates
	// are specified outside of mentioned range, texture wrapping option determines the look.
	// Each texture wrapping option can be set per coordinate axis (s, t and r if 3D textures are used).
	// s-axis, t-axis and r-axis correspond to x-axis, y-axis and z-axis, respectively.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters. Texture coordinates do not depend on resolution, but can be any
	// floating point value. Therefore, OpenGL needs to figure out which texture pixel (texel) to map the
	// texture coordinate to. Nearest neighbour filtering is better suited for minifying operations,
	// while (bi)linear filtering is better suited for magnifying operations.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Mipmaps are used to improve minifying, not magnifying. Setting one of the mipmap filtering options as
	// the magnification filter will generate the OpenGL "GL_INVALID_ENUM" error code.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Tell "stb_image.h" library to flip the y-axis during image loading. This call is necessary because
	// OpenGL expects the 0.0f coordinate on the y-axis to be on the bottom side of the image, but images
	// usually have 0.0f at the top of the y-axis.
	stbi_set_flip_vertically_on_load(true);

	// Load the image that will be used as a texture.
	int width;
	int height;
	int numberOfColorChannels;
	unsigned char* pixels = stbi_load("1_Getting_started/wooden_container.jpg", &width, &height,
		&numberOfColorChannels, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		// JPG image format doesn't include alpha (transparency) channel. We need to specify that to OpenGl, or
		// it will incorrectly interpret the image data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
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

	// Create memory on the GPU where second texture will be stored.
	unsigned int texture2;
	glGenTextures(1, &texture2);
	// Bind (assign) the newly created texture to OpenGL's context.
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set texture wrapping parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the image that will be used as a texture.
	pixels = stbi_load("1_Getting_Started/awesome_face.png", &width, &height, &numberOfColorChannels, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		// PNG image format includes alpha (transparency) channel. We need to specify that to OpenGl, or
		// it will incorrectly interpret the image data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		// Automatically generate all the required mipmaps for the currently bound texture.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Image of would-be-texture could not be loaded!" << std::endl;
		stbi_image_free(pixels);
		glfwTerminate();

		return 8;
	}
	// Free the image memory.
	stbi_image_free(pixels);

	// Unbind texture for safety reasons. This is not neccessary.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Activate the shader program.
	// Every shader and rendering call from now on will use this shader program object.
	ourShaderProgram.useProgram();

	// Retrieve location of uniform variable "transformationalMatrix" in shader program.
	// This doesn't require activation of shader program.
	int transformationalMatrixLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId,
		"transformationalMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (transformationalMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"transformationalMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 9;
	}

	// Tell OpenGL to which texture unit each shader sampler belongs to, by setting each sampler.
	glUniform1i(glGetUniformLocation(ourShaderProgram.shaderProgramId, "ourTexture1"), 0);
	ourShaderProgram.setIntegerUniform("ourTexture2", 1);

	// Retrieve location of uniform variable "mixFactor" in shader program.
	// This doesn't require activation of shader program.
	int mixFactorLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId, "mixFactor");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (mixFactorLocation == -1)
	{
		std::cout << "Location of uniform variable \"mixFactor\" wasn't found!" << std::endl;
		glfwTerminate();

		return 10;
	}

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// First part: Process the user's input.
		processInput_for_transformations(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate texture unit (one of 16). After activating a texture unit, a subsequent "glBindTexture"
		// call will bind that texture to the currently active texture unit. Texture unit "GL_TEXTURE0" is
		// always active by default, so it isn't necessary to manually activate any texture unit if only one
		// texture is used (like in previous example).
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Create a 4*4 matrix and initialize it with 1.0f on main diagonal, thus creating an identity matrix.
		glm::mat4 transformationalMatrix = glm::mat4(1.0f);
		// Transformations are meant to be read from right to left, which corresponds to bottom to top in code.
		// The order of transformations always has to be: scaling first, then rotation and finally translation.
		// Because we will pass matrices to each of the GLM's functions, GLM will automatically multiply the
		// matrices together, resulting in a transformational matrix that combines all the transformations.

		// Translate object to the bottom right corner of window.
		transformationalMatrix = glm::translate(transformationalMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
		// Rotate object over time around z-axis. GLM's "rotate" function requires the provided angle to be
		// specified in radians, so we convert the angle's value from degrees.
		// The axis we are rotating around should be a unit vector, so make sure to normalize the vector
		// representing the axis if we're not rotating around x, y or z-axis.
		transformationalMatrix = glm::rotate(transformationalMatrix, (float) glfwGetTime(), 
			glm::vec3(0.0f, 0.0f, 1.0f));
		/*
		transformationalMatrix = glm::rotate(transformationalMatrix, glm::radians(90.0f), 
			glm::vec3(0.0f, 0.0f, 1.0f));
		*/

		// Pass the transformational matrix to vertex shader (1 matrix, doesn't need to be transposed).
		glUniformMatrix4fv(transformationalMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformationalMatrix));

		// Set the uniform variable "mixFactor" in fragment shader.
		glUniform1f(mixFactorLocation, currentMixFactor);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
void framebuffer_size_callback_for_transformations(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input processing function.
void processInput_for_transformations(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Change how much the wooden container and awesome face are visible.
	// Increasing mixing factor will increase visibility of awesome face and decrease visibility of wooden container.
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		currentMixFactor += 0.01f;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixFactor >= 1.0f)
		{
			currentMixFactor = 1.0f;
		}
	}

	// Change how much the wooden container and awesome face are visible.
	// Decreasing mixing factor will increase visibility of wooden container and decrease visibility of awesome face.
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		currentMixFactor -= 0.01f;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixFactor <= 0.0f)
		{
			currentMixFactor = 0.0f;
		}
	}
}
