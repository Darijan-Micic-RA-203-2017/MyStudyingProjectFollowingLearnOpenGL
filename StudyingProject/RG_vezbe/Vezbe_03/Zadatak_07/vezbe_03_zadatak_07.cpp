#include "vezbe_03_zadatak_07.h"

int window_width_for_03_07 = 800;
int window_height_for_03_07 = 600;

const float triangleSideLength_for_03_07 = 0.1f;

/* Zadatak 7
Nacrtati dva šarena trougla, jedan iznad x-ose, a drugi ispod x-ose. Trouglovi se tokom vremena kreću po nevidljivoj
kružnici dovoljno malog poluprečnika tako da ne prelaze x-osu. Trougao iznad x-ose se kreće u smeru kazaljke na časovniku,
dok se trougao ispod x-ose kreće u suprotnom smeru. Boja trouglova postaje svetlija što su oni dalji od x-ose, odnosno
tamnija što su bliži x-osi.
*/
int draw_vezbe_03_zadatak_07()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_07, window_height_for_03_07, 
		"Vežbe 3 - zadatak 7", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_07);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_07/vertex_shader_for_03_07.glsl", 
		"Vezbe_03/Zadatak_07/fragment_shader_for_03_07.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float vertices[] = {
		// position                                                          // color
		// first triangle, set to be above the x-axis
		-triangleSideLength_for_03_07,  0.5f - triangleSideLength_for_03_07, 1.0f, 0.0f, 0.0f, 
		 triangleSideLength_for_03_07,  0.5f - triangleSideLength_for_03_07, 0.0f, 1.0f, 0.0f, 
		 0.0f,                          0.5f + triangleSideLength_for_03_07, 0.0f, 0.0f, 1.0f, 
		// second triangle, set to be below the x-axis
		-triangleSideLength_for_03_07, -0.5f + triangleSideLength_for_03_07, 1.0f, 0.0f, 0.0f, 
		 triangleSideLength_for_03_07, -0.5f + triangleSideLength_for_03_07, 0.0f, 1.0f, 0.0f, 
		 0.0f,                         -0.5f - triangleSideLength_for_03_07, 0.0f, 0.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_07(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_07(GLFWwindow* window, int width, int height)
{
	window_width_for_03_07 = width;
	window_height_for_03_07 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_07(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
