#include "vezbe_03_zadatak_06.h"

int window_width_for_03_06 = 800;
int window_height_for_03_06 = 600;

const float triangleSideLength_for_03_06 = 0.25f;

float movementOfTriangleOnXAxis_for_03_06 = -0.5f;
float movementOfTriangleOnYAxis_for_03_06 = 0.5f;

float deltaTime_for_03_06 = 0.0f;
float previousFrameTime_for_03_06 = 0.0f;

/* Zadatak 6
Nacrtati beli trougao na crnoj pozadini u gornjem levom kvadrantu ekrana, koji se pomera po ekranu sa WASD tasterima, a
vraća na koordinatni početak tasterom R. Deo trougla koji pređe u gornji desni kvadrant biva obojen crvenom bojom, deo koji
pređe u donji desni dio biva obojen ljubičastom bojom, a deo koji pređe u levi donji kvadrant biva obojen plavom bojom.
Deo trougla koji pređe na donju polovinu ekrana se crta u vertikalnim prugama. Ukoliko trougao priđe donjem desnom uglu,
ili se pritisne taster ESC, program se zatvara.
*/
int draw_vezbe_03_zadatak_06()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_06, window_height_for_03_06, 
		"Vežbe 3 - zadatak 6", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_06);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_06/vertex_shader_for_03_06.glsl", 
		"Vezbe_03/Zadatak_06/fragment_shader_for_03_06.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float vertices[] = {
		// position
		-triangleSideLength_for_03_06, -triangleSideLength_for_03_06, 
		 triangleSideLength_for_03_06, -triangleSideLength_for_03_06, 
		 triangleSideLength_for_03_06,  triangleSideLength_for_03_06
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_03_06 = currentFrameTime - previousFrameTime_for_03_06;
		previousFrameTime_for_03_06 = currentFrameTime;

		processInput_for_vezbe_03_zadatak_06(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update triangle position uniforms.
		shaderProgram.setFloatUniform("movementOfTriangleOnXAxis", movementOfTriangleOnXAxis_for_03_06);
		shaderProgram.setFloatUniform("movementOfTriangleOnYAxis", movementOfTriangleOnYAxis_for_03_06);

		glBindVertexArray(VAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	movementOfTriangleOnXAxis_for_03_06 = -0.5f;
	movementOfTriangleOnYAxis_for_03_06 = 0.5f;

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_06(GLFWwindow* window, int width, int height)
{
	window_width_for_03_06 = width;
	window_height_for_03_06 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_06(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float movementSpeed = 0.5f * deltaTime_for_03_06;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (movementOfTriangleOnYAxis_for_03_06 < 1.0f - triangleSideLength_for_03_06)
		{
			movementOfTriangleOnYAxis_for_03_06 += movementSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (movementOfTriangleOnYAxis_for_03_06 > -1.0f + triangleSideLength_for_03_06)
		{
			movementOfTriangleOnYAxis_for_03_06 -= movementSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (movementOfTriangleOnXAxis_for_03_06 > -1.0f + triangleSideLength_for_03_06)
		{
			movementOfTriangleOnXAxis_for_03_06 -= movementSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (movementOfTriangleOnXAxis_for_03_06 < 1.0f - triangleSideLength_for_03_06)
		{
			movementOfTriangleOnXAxis_for_03_06 += movementSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		movementOfTriangleOnXAxis_for_03_06 = 0.0f;
		movementOfTriangleOnYAxis_for_03_06 = 0.0f;
	}
}
