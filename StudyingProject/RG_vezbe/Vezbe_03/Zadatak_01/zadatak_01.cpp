#include "zadatak_01.h"

int window_width_for_3_1 = 800;
int window_height_for_3_1 = 600;

float colorOfBackground_for_3_1[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

/* Zadatak 1
Napisati program koji crta ljubičasti, jednakokraki trougao u gornjem desnom kvadrantu prozora, na tamno sivoj
pozadini. Baza trougla je duga 0.3 dužine jednog kvadranta prozora, a visina je 0.5 dužine jednog kvadranta.
Pritiskom na taster "B", pozadina menja boju u belu, a pritiskom na taster "S" se vraća na tamno sivu. Program se
zatvara pritiskom na taster "ESC".
*/
int draw_vezbe_03_zadatak_01()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_3_1, window_height_for_3_1, 
		"Vežbe 3 - zadatak 1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_01);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_01/vertex_shader_for_3_1.glsl", 
		"Vezbe_03/Zadatak_01/fragment_shader_for_3_1.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float vertices[] = {
		// position          // color
		-0.3f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
		 0.3f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
		 0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_01(window);

		glClearColor(colorOfBackground_for_3_1[0], colorOfBackground_for_3_1[1], colorOfBackground_for_3_1[2], 
			colorOfBackground_for_3_1[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		// The first two parameters of "glViewport" function are the coordinates of the bottom left corner of the
		// screen space, while the last two parameters are the width and the height of screen space.
		glViewport(window_width_for_3_1 / 2, window_height_for_3_1 / 2, 
			window_width_for_3_1 / 2, window_height_for_3_1 / 2);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_01(GLFWwindow* window, int width, int height)
{
	window_width_for_3_1 = width;
	window_height_for_3_1 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_01(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		for (unsigned int i = 0u; i < 4u; i++)
		{
			colorOfBackground_for_3_1[i] = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		for (unsigned int i = 0u; i < 3u; i++)
		{
			colorOfBackground_for_3_1[i] = 0.1f;
		}
		colorOfBackground_for_3_1[3] = 1.0f;
	}
}
