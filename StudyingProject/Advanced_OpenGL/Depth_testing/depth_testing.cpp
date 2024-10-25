#include "depth_testing.h"

const int window_width = 800;
const int window_height = 600;

float depthOfNearPlane_for_5_1_1 = 0.1f;
float depthOfFarPlane_for_5_1_1 = 100.0f;

Camera camera_for_5_1_1(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouseEntry_for_5_1_1 = true;
float previousCursorPosX_for_5_1_1 = (float) window_width / 2.0f;
float previousCursorPosY_for_5_1_1 = (float) window_height / 2.0f;

float deltaTime_for_5_1_1 = 0.0f;
float previousFrameTime_for_5_1_1 = 0.0f;

int draw_depth_testing()
{
	if (!glfwInit())
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, 
		"Advanced OpenGL - Depth testing", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_depth_testing);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_depth_testing);
	glfwSetScrollCallback(window, scroll_callback_for_depth_testing);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	// OpenGL allows modification of the comparison operators it uses for the depth testing of fragment.
	// Depth testing is a comparison of the fragment's depth value and the stored depth value. If the fragment
	// passes the depth test, the stored depth value will be replaced with the fragment's depth value.
	// Default value of "glDepthFunc" function is "GL_LESS" - fragment will pass the depth test if its depth value
	// is less than the stored depth value.
	// Depth buffer contains values in range [0.0f, 1.0f], but the fragment's z-value in view space is in range
	// [depth of near plane, depth of far plane]. The equation to transform the fragment's depth values in view
	// space is embedded into the projection matrix and it looks like this:
	// Fdepth = (1/z - 1/near) / (1/far - 1/near).
	// glDepthFunc(GL_ALWAYS);

	ShaderProgram ourShaderProgram("Depth_testing/vertex_shader_for_5_1_1.glsl", 
		"Depth_testing/fragment_shader_for_5_1_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}

	// Cube vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// In order to render a cube, we need 36 vertices (6 sides * 2 triangles per side * 3 vertices for each triangle).
	// First three values represent position of vertex, while last two values represent texture coordinates
	// (from 0.0f to 1.0f).
	// Vertex's surface is one of 6 planes forming a 3D cube.
	float verticesOfCube[] = {
		// position          // texture coordinates
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // back side
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 

		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // front side
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // left side
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 

		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // right side
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom side
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 

		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // top side
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f
	};
	// World space positions of our two cubes.
	glm::vec3 positionsOfCubes[] = {
		glm::vec3(-1.0f, 0.0f, -1.0f), 
		glm::vec3(2.0f, 0.0f, 0.0f)
	};
	// Plane vertices in world space coordinate system.
	// First three values represent position of vertex, while last two values represent texture coordinates
	// (from 0.0f to 1.0f).
	float verticesOfPlane[] = {
		// We intentionally specified texture coordinates in range [0.0f, 2.0f]. If texture's wrapping
		// parameters are set to "GL_REPEAT", texture will be repeated 2 times horizontally and vertically.
		// If they're set to "GL_CLAMP_TO_EDGE", texture will occupate 1/2 of horizontal and vertical space of
		// end vertices.
		// position          // texture coordinates
		 5.0f, -0.5f,  5.0f, 2.0f, 0.0f, 
		-5.0f, -0.5f,  5.0f, 0.0f, 0.0f, 
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f, 
		 5.0f, -0.5f,  5.0f, 2.0f, 0.0f, 
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f, 
		 5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfCube), verticesOfCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	unsigned int planeVAO;
	glGenVertexArrays(1, &planeVAO);
	unsigned int planeVBO;
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfPlane), verticesOfPlane, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	Texture marbleTexture("resources/marble.jpg");
	if (marbleTexture.errorCode)
	{
		glfwTerminate();

		return marbleTexture.errorCode;
	}
	Texture metalTexture("resources/metal.png");
	if (metalTexture.errorCode)
	{
		glfwTerminate();

		return metalTexture.errorCode;
	}

	glBindTexture(GL_TEXTURE_2D, 0u);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShaderProgram.useProgram();

	ourShaderProgram.setIntegerUniform("marbleTexture", 0);
	ourShaderProgram.setIntegerUniform("metalTexture", 1);

	ourShaderProgram.setFloatUniform("depthOfNearPlane", depthOfNearPlane_for_5_1_1);
	ourShaderProgram.setFloatUniform("depthOfFarPlane", depthOfFarPlane_for_5_1_1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_5_1_1 = currentFrameTime - previousFrameTime_for_5_1_1;
		previousFrameTime_for_5_1_1 = currentFrameTime;

		processInput_for_depth_testing(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, marbleTexture.id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, metalTexture.id);

		glm::mat4 projectionMatrix = glm::perspective(camera_for_5_1_1.fov, 
			(float) window_width / (float) window_height, depthOfNearPlane_for_5_1_1, depthOfFarPlane_for_5_1_1);
		ourShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);

		glm::mat4 viewMatrix = camera_for_5_1_1.getCalculatedViewMatrix();
		ourShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		glBindVertexArray(cubeVAO);
		bool marbleTextureNeeded = false;
		ourShaderProgram.setBoolUniform("marbleTextureNeeded", marbleTextureNeeded);
		glm::mat4 modelMatrix(1.0f);
		for (unsigned int i = 0u; i < 2u; i++)
		{
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, positionsOfCubes[i]);
			ourShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(planeVAO);
		marbleTextureNeeded = true;
		ourShaderProgram.setBoolUniform("marbleTextureNeeded", marbleTextureNeeded);
		modelMatrix = glm::mat4(1.0f);
		ourShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	firstMouseEntry_for_5_1_1 = true;

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_depth_testing(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void cursor_pos_callback_for_depth_testing(GLFWwindow* window, double xpos, double ypos)
{
	float xPos = static_cast<float>(xpos);
	float yPos = static_cast<float>(ypos);
	if (firstMouseEntry_for_5_1_1)
	{
		previousCursorPosX_for_5_1_1 = xPos;
		previousCursorPosY_for_5_1_1 = yPos;
		firstMouseEntry_for_5_1_1 = false;
	}

	float xOffset = xPos - previousCursorPosX_for_5_1_1;
	float yOffset = previousCursorPosY_for_5_1_1 - yPos;
	previousCursorPosX_for_5_1_1 = xPos;
	previousCursorPosY_for_5_1_1 = yPos;

	camera_for_5_1_1.processMouseMovement(xOffset, yOffset);
}

void scroll_callback_for_depth_testing(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_for_5_1_1.processMouseScroll(static_cast<float>(yoffset));
}

void processInput_for_depth_testing(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_for_5_1_1.processKeyboardInput("W", deltaTime_for_5_1_1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_for_5_1_1.processKeyboardInput("S", deltaTime_for_5_1_1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_for_5_1_1.processKeyboardInput("A", deltaTime_for_5_1_1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_for_5_1_1.processKeyboardInput("D", deltaTime_for_5_1_1);
	}
}
