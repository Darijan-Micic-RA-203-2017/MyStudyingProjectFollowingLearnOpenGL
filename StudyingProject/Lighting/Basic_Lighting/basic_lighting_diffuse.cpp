#include "basic_lighting_diffuse.h"

const int window_width = 800;
const int window_height = 600;

// Position of light source in world-space coordinates.
glm::vec3 positionOfLightSource_for_3_2_1(1.2f, 1.0f, 2.0f);

// All setting are kept in an instance of the camera class.
Camera camera_for_3_2_1(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouseEntry_for_3_2_1 = true;
float previousCursorPosX_for_3_2_1 = (float) window_width / 2.0f;
float previousCursorPosY_for_3_2_1 = (float) window_height / 2.0f;

// The time difference between the end of renderings of the current frame and the previous frame.
// We multiply all velocities with delta time value. The result is that when we have a large deltaTime in a frame,
// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to
// balance it all out. When using this approach it does not matter if you have a very fast or slow PC, the velocity
// of the camera will be balanced out accordingly so each user will have the same experience.
float deltaTime_for_3_2_1 = 0.0f;
// The time it took to render the previous frame.
float previousFrameTime_for_3_2_1 = 0.0f;

int draw_basic_lighting_diffuse()
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
		"Lighting - Basic Lighting, diffuse", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_basic_lighting_diffuse);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_basic_lighting_diffuse);
	glfwSetScrollCallback(window, scroll_callback_for_basic_lighting_diffuse);

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
	ShaderProgram ourShaderProgram("Basic_Lighting/vertex_shader_for_3_2_1.glsl", 
		"Basic_Lighting/fragment_shader_for_3_2_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}
	// Compile our light source shaders and link our light source shader program using helper class.
	ShaderProgram ourLightSourceShaderProgram("Basic_Lighting/light_source_vertex_shader_for_3_2_1.glsl", 
		"Basic_Lighting/light_source_fragment_shader_for_3_2_1.glsl");
	if (ourLightSourceShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourLightSourceShaderProgram.errorCode;
	}

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// We will turn our 2D plane into a 3D cube. In order to render a cube, we need 36 vertices
	// (6 sides * 2 triangles per side * 3 vertices for each triangle).
	// First three values represent position of vertex, while last three values represent vector normal to
	// vertex's surface. Vertex's surface is one of 6 planes forming a 3D cube.
	float vertices[] = {
		// Since a vertex by itself doesn't have a surface (vertex is simply a single point in space), we need to
		// use its surrounding vertices to figure out the surface of the vertex.
		// We can use a neat trick to calculate the normal vectors for all the 3D cube's vertices by using the
		// cross product, but since 3D cube is a simple shape we can simply manually add them to the vertex data.
		// position          // normal vector
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // back side
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // front side
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // left side
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // right side
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // bottom side
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top side
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);
	// Normal vector attribute.
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex normal vector attribute.
	glEnableVertexAttribArray(1u);

	// Create memory on the GPU where vertex data of light source will be stored.
	unsigned int lightSourceVAO;
	glGenVertexArrays(1, &lightSourceVAO);

	// Bind (assign) the newly created VAO to OpenGL's context.
	glBindVertexArray(lightSourceVAO);

	// Bind (assign) the previously created VBO to OpenGL's context. We use the same VBO, because the light source
	// object will use the same vertices as the object in scene (light source is also a 3D cube).
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	// Position attribute.
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// Nullth part: Calculate the new delta time and assign the current frame time to the previous frame time.
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_3_2_1 = currentFrameTime - previousFrameTime_for_3_2_1;
		previousFrameTime_for_3_2_1 = currentFrameTime;

		// First part: Process the user's input.
		processInput_for_basic_lighting_diffuse(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Since we're now using a depth buffer, we also want to clear it before each rendering iteration.
		// Otherwise, the depth information of the previous frame would remain in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate the shader program.
		// Every shader and rendering call from now on will use this shader program object.
		ourShaderProgram.useProgram();

		// The projection matrix transforms view space coordinates to clip space coordinates.
		// We will use the perspective projection with varying field of view (FOV) that user sets by scrolling,
		// 0.1f near plane and 100.0f far plane. Ratio of window's width and height is called the aspect ratio.
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera_for_3_2_1.fov), 
			(float) window_width / (float) window_height, 0.1f, 100.0f);
		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by moving the camera using the keyboard.
		glm::mat4 viewMatrix = camera_for_3_2_1.getCalculatedViewMatrix();
		// Set the view matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// The model matrix transforms local space coordinates to world space coordinates.
		// We won't perform any transformation of object (3D cube).
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		// Set the model matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

		// The normal matrix is a model matrix specifically tailored for normal vectors. Normal matrix is defined
		// as the transpose of the inverse of the upper-left 3x3 part of the model matrix.
		// Non-uniform scaling would transform vertex in such a way that the normal vector would no longer be
		// perpendicular to the vertex's surface. This means that the lighting of surface would be distorted. We
		// mitigate non-uniform scaling by multiplying normal vector with normal matrix.
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
		// Set the normal matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat3Uniform("normalMatrix", normalMatrix);

		// Set position of viewer to field "cameraPosition" of global object "camera".
		ourShaderProgram.setFloatVec3Uniform("positionOfViewer", camera_for_3_2_1.cameraPosition);
		// Set position of light source to global variable "positionOfLightSource".
		ourShaderProgram.setFloatVec3Uniform("positionOfLightSource", positionOfLightSource_for_3_2_1);
		// Set color of light source to white.
		ourShaderProgram.setFloatVec3Uniform("colorOfLightSource", glm::vec3(1.0f, 1.0f, 1.0f));
		// Set color of object to coral.
		ourShaderProgram.setFloatVec3Uniform("colorOfObject", glm::vec3(1.0f, 0.5f, 0.31f));

		// Render 3D cube.
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Activate the light source shader program.
		// Every shader and rendering call from now on will use this shader program object.
		ourLightSourceShaderProgram.useProgram();

		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourLightSourceShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);
		// Set the view matrix. This matrix changes each frame.
		ourLightSourceShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// The model matrix transforms local space coordinates to world space coordinates.
		// We will transform 3D cube that is object of our scene, thus copying it to become a representation of
		// our light source. We will scale it to 1/5 of its initial size and finally translate it to specified
		// position of light source.
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, positionOfLightSource_for_3_2_1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		// Set the model matrix. This matrix changes each frame.
		ourLightSourceShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

		// Render light source, represented by a 3D cube.
		glBindVertexArray(lightSourceVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightSourceVAO);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback functions.

// Function that will be called every time the application's window changes size.
void framebuffer_size_callback_for_basic_lighting_diffuse(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that will be called every time the user moves the mouse while the application has focus.
void cursor_pos_callback_for_basic_lighting_diffuse(GLFWwindow* window, double xpos, double ypos)
{
	// Calculate "camera's front" vector that acts as insurance that however we move, camera keeps looking
	// straight ahead. Math's explained below.
	// In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);

	// 0. step: if we received mouse input for the first time, we set the previous cursor position to the position
	// where the user entered the application window and calculate offsets based on it. Wihout this added step,
	// camera would suddenly jump to point of mouse entry, which is usually far away from window's center.
	if (firstMouseEntry_for_3_2_1)
	{
		previousCursorPosX_for_3_2_1 = static_cast<float>(xpos);
		previousCursorPosY_for_3_2_1 = static_cast<float>(ypos);
		firstMouseEntry_for_3_2_1 = false;
	}

	// 1. step: calculate the mouse's offset since last frame.
	float xOffset = static_cast<float>(xpos) - previousCursorPosX_for_3_2_1;
	// Order of subtraction is reversed, because y-coordinates range from bottom to top.
	float yOffset = previousCursorPosY_for_3_2_1 - static_cast<float>(ypos);
	previousCursorPosX_for_3_2_1 = static_cast<float>(xpos);
	previousCursorPosY_for_3_2_1 = static_cast<float>(ypos);

	// 2. step onward: done in Camera class.
	camera_for_3_2_1.processMouseMovement(xOffset, yOffset);
}

// Function that will be called every time the user scrolls the mouse's middle button.
void scroll_callback_for_basic_lighting_diffuse(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_for_3_2_1.processMouseScroll(static_cast<float>(yoffset));
}

// Input processing function.
void processInput_for_basic_lighting_diffuse(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_for_3_2_1.processKeyboardInput("W", deltaTime_for_3_2_1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_for_3_2_1.processKeyboardInput("S", deltaTime_for_3_2_1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_for_3_2_1.processKeyboardInput("A", deltaTime_for_3_2_1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_for_3_2_1.processKeyboardInput("D", deltaTime_for_3_2_1);
	}
}
