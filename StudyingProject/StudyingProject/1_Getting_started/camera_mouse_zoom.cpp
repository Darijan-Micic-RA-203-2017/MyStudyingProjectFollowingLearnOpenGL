#include "camera_mouse_zoom.h"

const int window_width = 800;
const int window_height = 600;

glm::vec3 cameraPosition_for_2_9_3 = glm::vec3(0.0f, 0.0f, 3.0f);
// This vector acts as insurance that however we move, camera keeps looking straight ahead. Math's explained below.
// In 2. thing we need to manually create LookAt matrix - "camera's direction":
// glm::vec3 cameraTarget = cameraPosition + cameraFront;
// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);
glm::vec3 cameraFront_for_2_9_3 = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 upVector_for_2_9_3 = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouseEntry_for_2_9_3 = true;
// We present camera's movement with Euler angles - 3 values that can represent any rotation in a 3D space.
// We will use first two of Eurler angles: pitch (top/bottom, movement around a fixed x-axis) and yaw (left/right,
// movement around a fixed y-axis). We won't calculate third's Euler angle, roll (movement around a fixed z-axis),
// because we won't rotate camera like a mobile phone, bending it to left or right or turning it upside-down.
float pitch_for_2_9_3 = 0.0f;
// Yaw is initialized to -90.0f, because a 0.0f value would result in "camera's direction" vector pointing to the
// right, towards the positive z-axis (on the xz plane). We don't want that, because we set up our camera to look
// away from user, towards the negative z-axis. Therefore, we initialize yaw Euler angle with a negative value,
// which means clockwise rotation.
float yaw_for_2_9_3 = -90.0f;
float previousCursorPosX_for_2_9_3 = (float) window_width / 2.0f;
float previousCursorPosY_for_2_9_3 = (float) window_height / 2.0f;
float fov_for_2_9_3 = 45.0f;

// The time difference between the end of renderings of the current frame and the previous frame.
// We multiply all velocities with delta time value. The result is that when we have a large deltaTime in a frame,
// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to
// balance it all out. When using this approach it does not matter if you have a very fast or slow PC, the velocity
// of the camera will be balanced out accordingly so each user will have the same experience. 
float deltaTime_for_2_9_3 = 0.0f;
// The time it took to render the previous frame.
float previousFrameTime_for_2_9_3 = 0.0f;

float currentMixingFactor_for_2_9_3 = 0.2f;

int draw_camera_mouse_zoom()
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
		"StudyingProject - Camera, looking around with mouse", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_camera_mouse_zoom);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_camera_mouse_zoom);
	glfwSetScrollCallback(window, scroll_callback_for_camera_mouse_zoom);

	// Tell GLFW library to capture and hide our mouse cursor. Capturing the mouse cursor means fixating it to the
	// center of application's window and only letting it move if application loses focus or quits.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Configure global OpenGL state.
	// Enable depth testing. Without the use of z-buffer, some sides of the cube will be drawn over other sides
	// of the cube. This happens because when OpenGL draws our cube triangle by triangle, fragment by fragment,
	// it will overwrite any pixel color that may have already been drawn there before. Since OpenGL gives no
	// guarantee on the order of triangles rendered (within the same draw call), some triangles are drawn on
	// top of each other even though one should clearly be in front of the other.
	// Luckily, OpenGL stores depth information in a buffer called the z-buffer that allows OpenGL to decide
	// when to or not to draw over a pixel. Using the z-buffer we can configure OpenGL to do depth testing.
	glEnable(GL_DEPTH_TEST);

	Shader ourShaderProgram("1_Getting_started/vertex_shader_for_2_9_1.glsl",
		"1_Getting_started/fragment_shader_for_2_9_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// We will turn our 2D plane into a 3D cube. In order to render a cube, we need 36 vertices
	// (6 sides * 2 triangles per side * 3 vertices for each triangle).
	// First three values represent position of vertex, while last two values represent texture coordinates
	// (from 0.0f to 1.0f).
	float vertices[] = {
		// position          // texture coordinates
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // back side
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // front side
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // left side
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // right side
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom side
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top side
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};
	// World space positions of our ten cubes.
	glm::vec3 positionsOfCubes[] = {
		glm::vec3(0.0f,  0.0f,  0.0f), 
		glm::vec3(2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f), 
		glm::vec3(-3.8f, -2.0f, -12.3f), 
		glm::vec3(2.4f, -0.4f, -3.5f), 
		glm::vec3(-1.7f,  3.0f, -7.5f), 
		glm::vec3(1.3f, -2.0f, -2.5f), 
		glm::vec3(1.5f,  2.0f, -2.5f), 
		glm::vec3(1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Create memory on the GPU where vertex data and index data will be stored.
	// Said data will be handled by VAO and vertex/element buffer objects inside that VAO.
	// Core OpenGL REQUIRES the use of VAOs!
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind (assign) the newly created VAO to OpenGL's context.
	glBindVertexArray(VAO);

	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0);
	// Texture coordinate attribute.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex texture coordinate attribute.
	glEnableVertexAttribArray(1);

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
		// JPG image format doesn't include alpha (transparency) channel. We need to specify that to OpenGL, or
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
	pixels = stbi_load("1_Getting_started/awesome_face.png", &width, &height, &numberOfColorChannels, 0);
	if (pixels)
	{
		// Generate a texture using the previously loaded image data (pixels).
		// PNG image format includes alpha (transparency) channel. We need to specify that to OpenGL, or
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

	// Retrieve location of uniform variable "modelMatrix" in shader program.
	// This doesn't require activation of shader program.
	int modelMatrixLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId, "modelMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (modelMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"modelMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 9;
	}

	// Retrieve location of uniform variable "viewMatrix" in shader program.
	// This doesn't require activation of shader program.
	int viewMatrixLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId, "viewMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (viewMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"viewMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 10;
	}

	// Retrieve location of uniform variable "projectionMatrix" in shader program.
	// This doesn't require activation of shader program.
	int projectionMatrixLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId, "projectionMatrix");
	// If uniform variable's location wasn't found, glGetUniformLocation returns -1.
	if (projectionMatrixLocation == -1)
	{
		std::cout << "Location of uniform variable \"projectionMatrix\" wasn't found!" << std::endl;
		glfwTerminate();

		return 11;
	}

	// Tell OpenGL to which texture unit each shader sampler belongs to, by setting each sampler.
	glUniform1i(glGetUniformLocation(ourShaderProgram.shaderProgramId, "ourTexture1"), 0);
	ourShaderProgram.setIntegerUniform("ourTexture2", 1);

	// Retrieve location of uniform variable "mixingFactor" in shader program.
	// This doesn't require activation of shader program.
	int mixingFactorLocation = glGetUniformLocation(ourShaderProgram.shaderProgramId, "mixingFactor");
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
		// Nullth part: Calculate the new delta time and assign the current frame time to the previous frame time.
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_2_9_3 = currentFrameTime - previousFrameTime_for_2_9_3;
		previousFrameTime_for_2_9_3 = currentFrameTime;

		// First part: Process the user's input.
		processInput_for_camera_mouse_zoom(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Since we're now using a depth buffer, we also want to clear it before each rendering iteration.
		// Otherwise, the depth information of the previous frame would remain in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate texture unit (one of 16). After activating a texture unit, a subsequent "glBindTexture"
		// call will bind that texture to the currently active texture unit. Texture unit "GL_TEXTURE0" is
		// always active by default, so it isn't necessary to manually activate any texture unit if only one
		// texture is used (like in examples previous to "Textures, combined").
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// The projection matrix transforms view space coordinates to clip space coordinates.
		// We will use the perspective projection with varying field of view (FOV) that user sets by scrolling,
		// 0.1f near plane and 100.0f far plane. Ratio of window's width and height is called the aspect ratio.
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov_for_2_9_3), 
			(float) window_width / (float) window_height, 0.1f, 100.0f);

		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by moving the camera using the keyboard.

		// MANUALLY create the LookAt (view) matrix.

		// 1. thing we need to create a LookAt matrix: the camera's position.
		// Camera's position is already set as the global variable and modified using WASD keys.

		// 2. thing we need to create a LookAt matrix: the "camera's direction". It's a bad name, because we
		// actually need the direction TO camera.
		// The camera's direction is counted by subtracting camera's position from the camera's target. However,
		// the camera in the OpenGL by convention points towards the negative z-axis and we want the z-axis in the
		// view matrix's coordinate system to be positive. Because of those two reasons, we switch the subtraction
		// order and subtract camera's target from camera's position. Vector visually ends at the minuend
		// (first operand of subtraction) and starts at the subtrahend (second operand of subtraction). Therefore,
		// we want it to end on camera's position, pointing to it.
		glm::vec3 cameraTarget = cameraPosition_for_2_9_3 + cameraFront_for_2_9_3;
		glm::vec3 cameraDirection = glm::normalize(cameraPosition_for_2_9_3 - cameraTarget);
		// 3. thing we need to create a LookAt matrix: the camera's right vector. This vector points right from
		// camera and is perpendicular to "camera's direction".
		// We can create it with the following trick. We first create an "up" vector, that's pointing upwards in
		// the global space (0.0f, 1.0f, 0.0f). Then, we create camera's right vector by doing a cross product
		// between "up" vector and "camera's direction". Result of a cross product is a vector perpendicular to
		// both vectors and we will get a vector that points in the positive x-axis' direction.
		glm::vec3 cameraRight = glm::normalize(glm::cross(upVector_for_2_9_3, cameraDirection));
		// 4. and final thing we need to create a LookAt matrix: the camera's up vector.
		// Since we have vectors that point in the positive z-axis' direction ("camera's direction") and the
		// positive x-axis's direction (camera's right vector), their cross product will give us the vector
		// pointing in the positive y-axis's direction (camera's up vector).
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

		// Form the manually made LookAt (view) matrix. We transform world coordinates to view coordinates.
		/* LookAt matrix:
		* ([Rx   Ux    Dx    0.0f])T * [1.0f 0.0f 0.0f -(Px)] = [Rx   Ry   Rz   0.0f] * [1.0f 0.0f 0.0f -Px ] =
		* ([Ry   Uy    Dy    0.0f])    [0.0f 1.0f 0.0f -(Py)]   [Ux   Uy   Uz   0.0f]   [0.0f 1.0f 0.0f -Py ]
		* ([Rz   Uz    Dz    0.0f])    [0.0f 0.0f 1.0f -(Pz)]   [Dx   Dy   Dz   0.0f]   [0.0f 0.0f 1.0f -Pz ]
		* ([0.0f 0.0f  0.0f  1.0f])    [0.0f 0.0f 0.0f  1.0f]   [0.0f 0.0f 0.0f 1.0f]   [0.0f 0.0f 0.0f 1.0f]
		* = [Rx   Ry   Rz   -Rx*Px - Ry*Py - Rz*Pz]
		*   [Ux   Uy   Uz   -Ux*Px - Uy*Py - Uz*Pz]
		*   [Dx   Dy   Dz   -Dx*Px - Dy*Py - Dz*Pz]
		*   [0.0f 0.0f 0.0f 1.0f                  ]
		*/
		glm::mat4 viewMatrix = glm::mat4(cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f, 
			cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f, 
			cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f) * glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 
				0.0f, 1.0f, 0.0f, 0.0f, 
				0.0f, 0.0f, 1.0f, 0.0f, 
				-cameraPosition_for_2_9_3.x, -cameraPosition_for_2_9_3.y, -cameraPosition_for_2_9_3.z, 1.0f);

		// Set the view matrix. This matrix changes each frame.
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Set the uniform variable "mixingFactor" in fragment shader.
		glUniform1f(mixingFactorLocation, currentMixingFactor_for_2_9_3);

		glBindVertexArray(VAO);
		// We draw ten cubes.
		for (unsigned int i = 0; i < 10; i++)
		{
			// The model matrix transforms local space coordinates to world space coordinates.
			// We will transform every third cube (including the first one) by rotating it over time around the
			// (1.0f, 0.3f, 0.5f) axis. The other cubes will only be rotated once over aforementioned axis.
			// Finally, each of the ten cubes will be translated to its corresponding specified position.
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, positionsOfCubes[i]);
			// GLM's "rotate" function requires the provided angle to be specified in radians, so we convert
			// the angle's value from degrees.
			// The axis we are rotating around should be a unit vector, so make sure to normalize the vector
			// representing the axis if we're not rotating around x, y or z-axis.
			float angle = 20.0f * i;
			if (i % 3 == 0)
			{
				angle = (float) glfwGetTime() * 25.0f;
			}
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle),
				glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));

			// Set the model matrix. This matrix changes each frame.
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback functions.

// Function that will be called every time the application's window changes size.
void framebuffer_size_callback_for_camera_mouse_zoom(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that will be called every time the user moves the mouse while the application has focus.
void cursor_pos_callback_for_camera_mouse_zoom(GLFWwindow* window, double xpos, double ypos)
{
	// Calculate "camera's front" vector that acts as insurance that however we move, camera keeps looking
	// straight ahead. Math's explained below.
	// In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);
	
	// 0. step: if we received mouse input for the first time, we set the previous cursor position to the position
	// where the user entered the application window and calculate offsets based on it. Wihout this added step,
	// camera would suddenly jump to point of mouse entry, which is usually far away from window's center.
	if (firstMouseEntry_for_2_9_3)
	{
		previousCursorPosX_for_2_9_3 = static_cast<float>(xpos);
		previousCursorPosY_for_2_9_3 = static_cast<float>(ypos);
		firstMouseEntry_for_2_9_3 = false;
	}

	// 1. step: calculate the mouse's offset since last frame.
	float xOffset = static_cast<float>(xpos) - previousCursorPosX_for_2_9_3;
	// Order of subtraction is reversed, because y-coordinates range from bottom to top.
	float yOffset = previousCursorPosY_for_2_9_3 - static_cast<float>(ypos);
	previousCursorPosX_for_2_9_3 = static_cast<float>(xpos);
	previousCursorPosY_for_2_9_3 = static_cast<float>(ypos);

	// 2. step: add the offset values to the camera's pitch and yaw values.
	// Mouse movement would be too erratic if we didn't scale it by a sensitivity variable.
	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	pitch_for_2_9_3 += yOffset;
	yaw_for_2_9_3 += xOffset;

	// 3. step: add constraints to the minimum and maximum pitch values.
	if (pitch_for_2_9_3 < -89.0f)
	{
		pitch_for_2_9_3 = -89.0f;
	}
	if (pitch_for_2_9_3 > 89.0f)
	{
		pitch_for_2_9_3 = 89.0f;
	}

	// 4. and final step: calculate ACTUAL camera's direction vector, the result of subtracting camera's position
	// from camera's target (thus visually ending at camera's target, minuend of subtraction).
	glm::vec3 direction = glm::vec3(0.0f);
	direction.x = cos(glm::radians(pitch_for_2_9_3)) * cos(glm::radians(yaw_for_2_9_3));
	direction.y = sin(glm::radians(pitch_for_2_9_3));
	direction.z = cos(glm::radians(pitch_for_2_9_3)) * sin(glm::radians(yaw_for_2_9_3));
	cameraFront_for_2_9_3 = glm::normalize(direction);
}

// Function that will be called every time the user scrolls the mouse's middle button.
void scroll_callback_for_camera_mouse_zoom(GLFWwindow* window, double xoffset, double yoffset)
{
	fov_for_2_9_3 -= static_cast<float>(yoffset);
	if (fov_for_2_9_3 < 1.0f)
	{
		fov_for_2_9_3 = 1.0f;
	}
	if (fov_for_2_9_3 > 45.0f)
	{
		fov_for_2_9_3 = 45.0f;
	}
}

// Input processing function.
void processInput_for_camera_mouse_zoom(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float mixingFactorChangeSpeed = 0.01f;
	// Change how much the wooden container and awesome face are visible.
	// Increasing mixing factor will increase visibility of awesome face and decrease visibility of wooden container.
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		currentMixingFactor_for_2_9_3 += mixingFactorChangeSpeed;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixingFactor_for_2_9_3 >= 1.0f)
		{
			currentMixingFactor_for_2_9_3 = 1.0f;
		}
	}

	// Change how much the wooden container and awesome face are visible.
	// Decreasing mixing factor will increase visibility of wooden container and decrease visibility of awesome face.
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		currentMixingFactor_for_2_9_3 -= mixingFactorChangeSpeed;
		// Prevent falling out of allowed range of mixing factor.
		if (currentMixingFactor_for_2_9_3 <= 0.0f)
		{
			currentMixingFactor_for_2_9_3 = 0.0f;
		}
	}

	// The camera will move at a constant speed of 2.5 units per second.
	float cameraSpeed = 2.5f * deltaTime_for_2_9_3;
	// Move camera forward (away from yourself, in negative z-axis' direction) by adding scaled camera's
	// direction to camera's position.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition_for_2_9_3 += cameraFront_for_2_9_3 * cameraSpeed;
	}

	// Move camera backwards (towards yourself, in positive z-axis' direction) by subtracting scaled camera's
	// direction from camera's position.
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition_for_2_9_3 -= cameraFront_for_2_9_3 * cameraSpeed;
	}

	// Move camera to the left (in negative x-axis' direction) by subtracting scaled right vector (cross product of
	// camera's front vector and "up" vector) from camera's position. We have to normalize that right vector,
	// because cross product could return differently sized vectors depending on the value of "cameraFront". Also,
	// we would move slow or fast depending on camera's orientation, instead of at a consistent speed.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition_for_2_9_3 -= glm::normalize(glm::cross(cameraFront_for_2_9_3, upVector_for_2_9_3)) * cameraSpeed;
	}

	// Move camera to the right (in positive x-axis' direction) by adding scaled right vector (cross product of
	// camera's front vector and "up" vector) to camera's position. We have to normalize that right vector,
	// because cross product could return differently sized vectors depending on the value of "cameraFront". Also,
	// we would move slow or fast depending on camera's orientation, instead of at a consistent speed.
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition_for_2_9_3 += glm::normalize(glm::cross(cameraFront_for_2_9_3, upVector_for_2_9_3)) * cameraSpeed;
	}
}
