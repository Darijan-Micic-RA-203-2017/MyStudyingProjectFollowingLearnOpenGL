#include "vezbe_03_zadatak_05.h"

int window_width_for_03_05 = 800;
int window_height_for_03_05 = 600;

float pointSize_for_03_05 = 7.0f;

float movementOfPointOnXAxis_for_03_05 = 0.0f;
float movementOfPointOnYAxis_for_03_05 = 0.0f;

float deltaTime_for_03_05 = 0.0f;
float previousFrameTime_for_03_05 = 0.0f;

/* Zadatak 5
Napisati aplikaciju čiji je ekranski prostor ceo ekran i koja prikazuje crvenu tačku na centru ekrana. Tačka je veća od 1
piksela i može se pomerati po ekranu pomoću WASD tastera, a ukoliko izađe izvan ekrana, pojavljuje se na suprotnoj ivici.
Boja tačke pulsira od crvene do žute tokom vremena, i pulsira brže što je tačka dalja od koordinatnog početka.
Tačka ostavlja trag trenutne boje za sobom.
Taster ESC zatvara program. Taster R briše ekran i resetuje tačku na koordinatni početak.
*/
int draw_vezbe_03_zadatak_05()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*
	GLFWwindow* window = glfwCreateWindow(window_width_for_03_05, window_height_for_03_05, 
		"Vežbe 3 - zadatak 5", glfwGetPrimaryMonitor(), NULL);
	*/
	GLFWwindow* window = glfwCreateWindow(window_width_for_03_05, window_height_for_03_05, 
		"Vežbe 3 - zadatak 5", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_05);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Configure global OpenGL state.
	// Enable programmable point size.
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(pointSize_for_03_05);

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_05/vertex_shader_for_03_05.glsl", 
		"Vezbe_03/Zadatak_05/fragment_shader_for_03_05.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float verticesOfPoint[] = {
		// position   // color
		 0.0f,  0.0f, 1.0f, 0.0f, 0.0f
	};

	unsigned int pointVAO;
	glGenVertexArrays(1, &pointVAO);
	unsigned int pointVBO;
	glGenBuffers(1, &pointVBO);

	glBindVertexArray(pointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfPoint), verticesOfPoint, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_03_05 = currentFrameTime - previousFrameTime_for_03_05;
		previousFrameTime_for_03_05 = currentFrameTime;

		processInput_for_vezbe_03_zadatak_05(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update point position uniforms.
		shaderProgram.setFloatUniform("movementOfPointOnXAxis", movementOfPointOnXAxis_for_03_05);
		shaderProgram.setFloatUniform("movementOfPointOnYAxis", movementOfPointOnYAxis_for_03_05);

		glBindVertexArray(pointVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_05(GLFWwindow* window, int width, int height)
{
	window_width_for_03_05 = width;
	window_height_for_03_05 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_05(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float movementSpeed = 0.75f * deltaTime_for_03_05;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movementOfPointOnYAxis_for_03_05 += movementSpeed;

		// Make the point emerge on the opposite side of the screen space when the screen space's boundaries are crossed.
		if (movementOfPointOnYAxis_for_03_05 > 1.0f)
		{
			movementOfPointOnYAxis_for_03_05 = -1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		movementOfPointOnYAxis_for_03_05 -= movementSpeed;

		// Make the point emerge on the opposite side of the screen space when the screen space's boundaries are crossed.
		if (movementOfPointOnYAxis_for_03_05 < -1.0f)
		{
			movementOfPointOnYAxis_for_03_05 = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movementOfPointOnXAxis_for_03_05 -= movementSpeed;
		// Make the point emerge on the opposite side of the screen space when the screen space's boundaries are crossed.
		if (movementOfPointOnXAxis_for_03_05 < -1.0f)
		{
			movementOfPointOnXAxis_for_03_05 = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movementOfPointOnXAxis_for_03_05 += movementSpeed;

		// Make the point emerge on the opposite side of the screen space when the screen space's boundaries are crossed.
		if (movementOfPointOnXAxis_for_03_05 > 1.0f)
		{
			movementOfPointOnXAxis_for_03_05 = -1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		movementOfPointOnXAxis_for_03_05 = 0.0f;
		movementOfPointOnYAxis_for_03_05 = 0.0f;
	}
}
