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
	// glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// Star has 5 vertices, but I have to add 2 more for its center and another drawing of vertex at angle of 0.0f degrees.
	float verticesOfStar[7u * 5u + 2u * 7u];
	// Set the radius of the circle.
	float r = 0.175f;
	verticesOfStar[0u] = 0.7f; verticesOfStar[1u] = 0.7f; verticesOfStar[2u] = 0.0f; // position
	verticesOfStar[3u] = 0.0f; verticesOfStar[4u] = 1.0f; verticesOfStar[5u] = 1.0f; verticesOfStar[6u] = 1.0f; // color
	for (unsigned int i = 0u; i <= 5u; i++)
	{
		// position
		verticesOfStar[7u + 7u * i] = r * cos((3.141592f / 180.0f) * (i * (360.0f / 5.0f))) + 0.7f;
		verticesOfStar[7u + 7u * i + 1u] = r * sin((3.141592f / 180.0f) * (i * (360.0f / 5.0f))) + 0.7f;
		verticesOfStar[7u + 7u * i + 2u] = 0.0f;
		// color
		verticesOfStar[7u + 7u * i + 3u] = 0.0f;
		verticesOfStar[7u + 7u * i + 4u] = 0.0f;
		verticesOfStar[7u + 7u * i + 5u] = 1.0f;
		verticesOfStar[7u + 7u * i + 6u] = 1.0f;
	}

	unsigned int triangleVAO, squareVAO, starVAO;
	glGenVertexArrays(1, &triangleVAO);
	glGenVertexArrays(1, &squareVAO);
	glGenVertexArrays(1, &starVAO);
	unsigned int triangleVBO, squareVBO, starVBO;
	glGenBuffers(1, &triangleVBO);
	glGenBuffers(1, &squareVBO);
	glGenBuffers(1, &starVBO);

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

	glBindVertexArray(starVAO);

	glBindBuffer(GL_ARRAY_BUFFER, starVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfStar), verticesOfStar, GL_STATIC_DRAW);

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

		glBindVertexArray(starVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLE_FAN, 0, 7);

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
