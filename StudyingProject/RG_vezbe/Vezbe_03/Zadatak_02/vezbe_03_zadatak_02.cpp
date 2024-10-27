#include "vezbe_03_zadatak_02.h"

int window_width_for_03_02 = 800;
int window_height_for_03_02 = 600;

/* Zadatak 2
Napisati program koji na levoj polovini ekrana crta zastavu države po proizvoljnom izboru, a na desnoj polovini prozora
crta zastavu Japana.
*/
int draw_vezbe_03_zadatak_02()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_02, window_height_for_03_02, 
		"Vežbe 3 - zadatak 2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_02);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_02/vertex_shader_for_03_02.glsl", 
		"Vezbe_03/Zadatak_02/fragment_shader_for_03_02.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float verticesOfSerbianFlag[] = {
		// position          // color
		-1.0f,  0.32f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // red stripe
		 1.0f,  0.32f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		 1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		-1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		-1.0f, -0.34f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // blue stripe
		 1.0f, -0.34f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		 1.0f,  0.32f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		-1.0f,  0.32f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		-1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // white stripe
		 1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
		 1.0f, -0.34f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
		-1.0f, -0.34f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};
	unsigned int indicesOfSerbianFlag[] = {
		0u, 1u,  3u, 
		1u, 2u,  3u, 
		4u, 5u,  7u, 
		5u, 6u,  7u, 
		8u, 9u,  11u, 
		9u, 10u, 11u
	};

	unsigned int serbianFlagVAO;
	glGenVertexArrays(1, &serbianFlagVAO);
	unsigned int serbianFlagVBO;
	glGenBuffers(1, &serbianFlagVBO);
	unsigned int serbianFlagEBO;
	glGenBuffers(1, &serbianFlagEBO);

	glBindVertexArray(serbianFlagVAO);

	glBindBuffer(GL_ARRAY_BUFFER, serbianFlagVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfSerbianFlag), verticesOfSerbianFlag, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, serbianFlagEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesOfSerbianFlag), indicesOfSerbianFlag, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_02(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// The first two parameters of "glViewport" function are the coordinates of the bottom left corner of the
		// screen space, while the last two parameters are the width and the height of screen space.
		glViewport(0, 0, window_width_for_03_02 / 2, window_height_for_03_02);
		glBindVertexArray(serbianFlagVAO);
		glDrawElements(GL_TRIANGLES, sizeof(indicesOfSerbianFlag), GL_UNSIGNED_INT, (void*) 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_02(GLFWwindow* window, int width, int height)
{
	window_width_for_03_02 = width;
	window_height_for_03_02 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_02(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
