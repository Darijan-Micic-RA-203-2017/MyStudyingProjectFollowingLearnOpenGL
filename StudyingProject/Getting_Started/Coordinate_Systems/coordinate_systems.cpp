#include "coordinate_systems.h"

const int window_width = 800;
const int window_height = 600;

float currentMixingFactor_for_2_8_1 = 0.2f;

int draw_coordinate_systems()
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
		"Getting Started - Coordinate Systems", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_coordinate_systems);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Compile our shaders and link our shader program using helper class.
	ShaderProgram ourShaderProgram("Coordinate_Systems/vertex_shader_for_2_8_1.glsl", "Coordinate_Systems/fragment_shader_for_2_8_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// First three values represent position of vertex, while last two values represent texture coordinates
	// (from 0.0f to 1.0f).
	float vertices[] = {
		// position         // texture coordinates
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left
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
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);
	// Texture coordinate attribute.
	glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex texture coordinate attribute.
	glEnableVertexAttribArray(1u);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

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
	int textureImageWidth;
	int textureImageHeight;
	int numberOfColorChannelsInTextureImage;
	unsigned char* pixels = stbi_load("resources/wooden_container.jpg", &textureImageWidth, &textureImageHeight, 
		&numberOfColorChannelsInTextureImage, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		// JPG image format doesn't include alpha (transparency) channel. We need to specify that to OpenGL, or
		// it will incorrectly interpret the image data.
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
	pixels = stbi_load("resources/awesome_face.png", &textureImageWidth, &textureImageHeight, 
		&numberOfColorChannelsInTextureImage, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		// PNG image format includes alpha (transparency) channel. We need to specify that to OpenGL, or
		// it will incorrectly interpret the image data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImageWidth, textureImageHeight, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, pixels);
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
	glBindTexture(GL_TEXTURE_2D, 0u);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Activate the shader program.
	// Every shader and rendering call from now on will use this shader program object.
	ourShaderProgram.useProgram();

	// Retrieve location of uniform variable "modelMatrix" in shader program.
	// This doesn't require activation of shader program.
	int modelMatrixLocation = glGetUniformLocation(ourShaderProgram.id, "modelMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (modelMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"modelMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 9;
	}

	// Retrieve location of uniform variable "viewMatrix" in shader program.
	// This doesn't require activation of shader program.
	int viewMatrixLocation = glGetUniformLocation(ourShaderProgram.id, "viewMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (viewMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"viewMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 10;
	}

	// Retrieve location of uniform variable "projectionMatrix" in shader program.
	// This doesn't require activation of shader program.
	int projectionMatrixLocation = glGetUniformLocation(ourShaderProgram.id, "projectionMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (projectionMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"projectionMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 11;
	}
	// The projection matrix transforms view space coordinates to clip space coordinates.
	// We will use the perspective projection with standard 45 degrees field of view (FOV), 0.1f near plane and
	// 100.0f far plane. Ratio of window's width and height is called the aspect ratio.
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 
		(float) (window_width) / (float) (window_height), 0.1f, 100.0f);
	// Projection matrix rarely changes, so it's best practice to set it once outside the rendering loop.
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0u][0u]);

	// Tell OpenGL to which texture unit each shader sampler belongs to, by setting each sampler.
	glUniform1i(glGetUniformLocation(ourShaderProgram.id, "ourTexture1"), 0);
	ourShaderProgram.setIntegerUniform("ourTexture2", 1);

	// Retrieve location of uniform variable "mixingFactor" in shader program.
	// This doesn't require activation of shader program.
	int mixingFactorLocation = glGetUniformLocation(ourShaderProgram.id, "mixingFactor");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (mixingFactorLocation == -1)
	{
		std::cout << "Location of uniform variable \"mixingFactor\" wasn't found!" << std::endl;
		glfwTerminate();

		return 12;
	}

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// First part: Process the user's input.
		processInput_for_coordinate_systems(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate texture unit (one of 16). After activating a texture unit, a subsequent "glBindTexture"
		// call will bind that texture to the currently active texture unit. Texture unit "GL_TEXTURE0" is
		// always active by default, so it isn't necessary to manually activate any texture unit if only one
		// texture is used (like in examples previous to "Textures, combined").
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// The model matrix transforms local space coordinates to world space coordinates.
		// We will transform our plane by rotating it on the x-axis so it looks like it's laying on the floor.
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		// GLM's "rotate" function requires the provided angle to be specified in radians, so we convert the
		// angle's value from degrees.
		// The axis we are rotating around should be a unit vector, so make sure to normalize the vector
		// representing the axis if we're not rotating around x, y or z-axis.
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by translating it forward, which equals moving the camera
		// backwards. Keep in mind we need to translate the world in the inverse direction of where we want the
		// camera to move.
		// By convention, OpenGL is a right-handed system. That means the negative z-axis is going into the
		// screen away from the user, while the positive z-axis is going through the screen towards the user.
		// Because we want to move backwards and since OpenGL is a right-handed system, we have to move in the
		// positive z-axis. We do this by translating the scene towards the negative z-axis. This gives the
		// impression that we are moving backwards.
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

		// Set the model matrix and view matrix. These two matrices change each frame.
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Set the uniform variable "mixingFactor" in fragment shader.
		glUniform1f(mixingFactorLocation, currentMixingFactor_for_2_8_1);

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
void framebuffer_size_callback_for_coordinate_systems(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input processing function.
void processInput_for_coordinate_systems(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Change how much the first texture and the second texture are visible.
	// Increasing mixing factor will increase visibility of second texture and decrease visibility of first texture.
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		currentMixingFactor_for_2_8_1 += 0.01f;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixingFactor_for_2_8_1 >= 1.0f)
		{
			currentMixingFactor_for_2_8_1 = 1.0f;
		}
	}

	// Change how much the first texture and the second texture are visible.
	// Decreasing mixing factor will increase visibility of first texture and decrease visibility of second texture.
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		currentMixingFactor_for_2_8_1 -= 0.01f;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixingFactor_for_2_8_1 <= 0.0f)
		{
			currentMixingFactor_for_2_8_1 = 0.0f;
		}
	}
}
