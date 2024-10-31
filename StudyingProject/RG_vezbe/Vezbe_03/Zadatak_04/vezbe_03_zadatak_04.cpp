#include "vezbe_03_zadatak_04.h"

int window_width_for_03_04 = 800;
int window_height_for_03_04 = 600;

/* Zadatak 4
Nacrtati proizvoljnu plavu zvezdu na proizvoljnoj poziciji. Nacrtati poluprovidan kvadrat na centru ekrana, čija je
stranica duga 20% jednog kvadranta i uraditi sledeće funkcionalnosti tastera:
ESC - zatvara program;
Strelice - pomeranje kvadrata, tako da kvadrat ne može preći ivice prozora;
R - vraća kvadrat na centar ekrana;
1 - prikazuje samo temena poligona koji čine kvadrat, koja su povećana da bi bila lakše uočljiva;
2 - prikazuje ivice poligona (tzv. wireframe mode);
3 - prikazuje kvadrat ispunjen bojom;
4 - isključuje providnost kvadrata;
5 - uključuje providnost kvadrata.
Kvadrat se crta preko trougla, i na trougao ne utiču funkcionalnosti tastera 1 - 5.
*/
int draw_vezbe_03_zadatak_04()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_04, window_height_for_03_04, 
		"Vežbe 3 - zadatak 4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_04);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Configure global OpenGL state.
	// Enable blending.
	glEnable(GL_BLEND);
	// We blend colors by setting the blending function with the "glBlendFunc" function. Its parameters:
	// - the source color factor (the factor of the output variable of the fragment shader);
	// - the destination color factor (the factor of the color of the fragment we are drawing over).
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_04/vertex_shader_for_03_04.glsl", 
		"Vezbe_03/Zadatak_04/fragment_shader_for_03_04.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float verticesOfTriangle[] = {
		// position        // color
		-0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		 0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		 0.0f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	float verticesOfSquare[] = {
		// position         // color
		-0.4f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 
		 0.4f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 
		-0.4f,  0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 
		-0.4f,  0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 
		 0.4f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 
		 0.4f,  0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f
	};

	unsigned int triangleVAO, squareVAO;
	glGenVertexArrays(1, &triangleVAO);
	glGenVertexArrays(1, &squareVAO);
	unsigned int triangleVBO, squareVBO;
	glGenBuffers(1, &triangleVBO);
	glGenBuffers(1, &squareVBO);

	glBindVertexArray(triangleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfTriangle), verticesOfTriangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindVertexArray(squareVAO);

	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfSquare), verticesOfSquare, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_04(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(triangleVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(squareVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_04(GLFWwindow* window, int width, int height)
{
	window_width_for_03_04 = width;
	window_height_for_03_04 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_04(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
