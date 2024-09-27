#include "model_loading.h"

const int window_width = 800;
const int window_height = 600;

// All settings are kept in an instance of the camera class.
Camera camera_for_4_3_1(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouseEntry_for_4_3_1 = true;
float previousCursorPosX_for_4_3_1 = (float) window_width / 2.0f;
float previousCursorPosY_for_4_3_1 = (float) window_height / 2.0f;

// The time difference between the end of renderings of the current frame and the previous frame.
// We multiply all velocities with delta time value. The result is that when we have a large deltaTime in a frame,
// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to
// balance it all out. When using this approach it does not matter if you have a very fast or slow PC, the velocity
// of the camera will be balanced out accordingly so each user will have the same experience.
float deltaTime_for_4_3_1 = 0.0f;
// The time it took to render the previous frame.
float previousFrameTime_for_4_3_1 = 0.0f;

int draw_model_loading()
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
		"Model Loading - Backpack", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_model_loading);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_model_loading);
	glfwSetScrollCallback(window, scroll_callback_for_model_loading);

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

	// Compile our shaders and link our shader program using helper class.
	ShaderProgram ourShaderProgram("Model/vertex_shader_for_4_3_1.glsl", 
		"Model/fragment_shader_for_4_3_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}

	// Tell "stb_image.h" library to flip the y-axis during image loading. This call is necessary because
	// OpenGL expects the 0.0f coordinate on the y-axis to be on the bottom side of the image, but images
	// usually have 0.0f at the top of the y-axis.
	stbi_set_flip_vertically_on_load(true);

	// Load the model, translate it into the "Assimp" data structure, copy its vertices and indices to GPU and
	// generate textures of its material.
	Model backpack("resources/backpack/backpack.obj");

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

	// Activate the shader program.
	// Every shader and rendering call from now on will use this shader program object.
	ourShaderProgram.useProgram();

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// Nullth part: Calculate the new delta time and assign the current frame time to the previous frame time.
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_4_3_1 = currentFrameTime - previousFrameTime_for_4_3_1;
		previousFrameTime_for_4_3_1 = currentFrameTime;

		// First part: Process the user's input.
		processInput_for_model_loading(window);

		// Second part: Rendering commands.
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		// Since we're now using a depth buffer, we also want to clear it before each rendering iteration.
		// Otherwise, the depth information of the previous frame would remain in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// The projection matrix transforms view space coordinates to clip space coordinates.
		// We will use the perspective projection with varying field of view (FOV) that user sets by scrolling,
		// 0.1f near plane and 100.0f far plane. Ratio of window's width and height is called the aspect ratio.
		glm::mat4 projectionMatrix = glm::perspective(camera_for_4_3_1.fov, 
			(float) window_width / (float) window_height, 0.1f, 100.0f);
		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by moving the camera using the keyboard.
		glm::mat4 viewMatrix = camera_for_4_3_1.getCalculatedViewMatrix();
		// Set the view matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// The model matrix transforms local space coordinates to world space coordinates.
		// We won't perform any transformations.
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
		// Set the model matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

		// Render backpack.
		backpack.drawUsing(ourShaderProgram);

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// On next drawing, reset first mouse entry indicator.
	firstMouseEntry_for_4_3_1 = true;

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback functions.

// Function that will be called every time the application's window changes size.
void framebuffer_size_callback_for_model_loading(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that will be called every time the user moves the mouse while the application has focus.
void cursor_pos_callback_for_model_loading(GLFWwindow* window, double xpos, double ypos)
{
	// Calculate "camera's front" vector that acts as insurance that however we move, camera keeps looking
	// straight ahead. Math's explained below.
	// In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);

	// 0. step: if we received mouse input for the first time, we set the previous cursor position to the position
	// where the user entered the application window and calculate offsets based on it. Wihout this added step,
	// camera would suddenly jump to point of mouse entry, which is usually far away from window's center.
	float xPos = static_cast<float>(xpos);
	float yPos = static_cast<float>(ypos);
	if (firstMouseEntry_for_4_3_1)
	{
		previousCursorPosX_for_4_3_1 = xPos;
		previousCursorPosY_for_4_3_1 = yPos;
		firstMouseEntry_for_4_3_1 = true;
	}

	// 1. step: calculate the mouse's offset since last frame.
	float xOffset = xPos - previousCursorPosX_for_4_3_1;
	// Order of subtraction is reversed, because y-coordinates range from bottom to top.
	float yOffset = previousCursorPosY_for_4_3_1 - yPos;
	previousCursorPosX_for_4_3_1 = xPos;
	previousCursorPosY_for_4_3_1 = yPos;

	// 2. step onward: done in Camera class.
	camera_for_4_3_1.processMouseMovement(xOffset, yOffset);
}

// Function that will be called every time the user scrolls the mouse's middle button.
void scroll_callback_for_model_loading(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_for_4_3_1.processMouseScroll(static_cast<float>(yoffset));
}

// Input processing function.
void processInput_for_model_loading(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_for_4_3_1.processKeyboardInput("W", deltaTime_for_4_3_1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_for_4_3_1.processKeyboardInput("S", deltaTime_for_4_3_1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_for_4_3_1.processKeyboardInput("A", deltaTime_for_4_3_1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_for_4_3_1.processKeyboardInput("D", deltaTime_for_4_3_1);
	}
}
