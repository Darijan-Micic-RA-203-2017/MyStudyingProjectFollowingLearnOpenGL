#define NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE 5u
// Number of discrete points the circle consists of. The circle is a set of connected points bounding the disc (plane area).
// Judging from my own observations, the circle seems round to the eye when its resolution is greater than or equal to 24.
#define RESOLUTION_OF_CIRCLE 28u

#include "vezbe_03_zadatak_03.h"

int window_width_for_03_03 = 500;
int window_height_for_03_03 = 500;

/* Zadatak 3
Napisati program koji crta šarenu traku proizvoljnih boja oko ivica prozora koji je dimenzija 500x500, a na sredini ekrana
prvo slovo svog imena proizvoljne boje na svetlo sivoj pozadini. Širina trake iznosi 30 % širine jednog kvadranta. Debljina
linija slova mora biti veća od 1 piksela. Boja pozadine se tokom vremena smenjuje od potpuno bele do potpuno crne boje.
*/
int draw_vezbe_03_zadatak_03()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_03, window_height_for_03_03, 
		"Vežbe 3 - zadatak 3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_03);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Set the width of lines to 7 pixels.
	glLineWidth(7.0f);

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_03/vertex_shader_for_03_03.glsl", 
		"Vezbe_03/Zadatak_03/fragment_shader_for_03_03.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float verticesOfBoundingStripe[] = {
		// When drawing using the "GL_TRIANGLE_STRIP" primitive, vertices should be ordered so that the last two vertices
		// of the previous triangle consist a triangle coupled with the following vertex. In order to compensate for the
		// change in face orientation, OpenGL will automatically change the order of drawing so the user doesn't have to
		// worry about that.
		// position   // color
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // v0
		 0.7f,  0.7f, 0.5f, 0.0f, 0.5f, // v1
		 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // v2
		 0.7f, -0.7f, 0.5f, 0.2f, 1.0f, // v3
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // v4
		-0.7f, -0.7f, 0.0f, 0.5f, 0.5f, // v5
		-1.0f,  1.0f, 0.5f, 0.1f, 0.5f, // v6
		-0.7f,  0.7f, 0.5f, 0.0f, 0.5f, // v7
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // v8
		 0.7f,  0.7f, 0.5f, 0.0f, 0.5f  // v9
	};

	// I only need the right half of the circle, a.k.a. its fourth and first quadrant.
	const unsigned int sizeOfArrayOfVerticesOfLetterD = 
		NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * (RESOLUTION_OF_CIRCLE / 2u + 1u);
	float verticesOfLetterD[sizeOfArrayOfVerticesOfLetterD];
	// Set the radius of the circle.
	float r = 0.35f;
	for (int i = -static_cast<int>(RESOLUTION_OF_CIRCLE) / 4; i <= static_cast<int>(RESOLUTION_OF_CIRCLE) / 4; i++)
	{
		unsigned int locationInArrayOfVertices = static_cast<unsigned int>(i + RESOLUTION_OF_CIRCLE / 4);

		// position
		verticesOfLetterD[NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * locationInArrayOfVertices] = 
			r * cos((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE))) - r / 2.0f;
		verticesOfLetterD[NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * locationInArrayOfVertices + 1u] = 
			r * sin((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE)));
		// color
		verticesOfLetterD[NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * locationInArrayOfVertices + 2u] = 0.0f;
		verticesOfLetterD[NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * locationInArrayOfVertices + 3u] = 1.0f;
		verticesOfLetterD[NUMBER_OF_DATA_IN_VERTEX_OF_CIRCLE * locationInArrayOfVertices + 4u] = 0.0f;
	}

	unsigned int boundingStripeVAO, letterDVAO;
	glGenVertexArrays(1, &boundingStripeVAO);
	glGenVertexArrays(1, &letterDVAO);
	unsigned int boundingStripeVBO, letterDVBO;
	glGenBuffers(1, &boundingStripeVBO);
	glGenBuffers(1, &letterDVBO);

	glBindVertexArray(boundingStripeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, boundingStripeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfBoundingStripe), verticesOfBoundingStripe, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindVertexArray(letterDVAO);

	glBindBuffer(GL_ARRAY_BUFFER, letterDVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfLetterD), verticesOfLetterD, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_03(window);

		float colorOfBackground = static_cast<float>(abs(cos(glfwGetTime())));
		glClearColor(colorOfBackground, colorOfBackground, colorOfBackground, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(boundingStripeVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(verticesOfBoundingStripe) / (5 * sizeof(float)));
		// We draw 10 vertices, which is the quotient (the result of the division) of the size of bounding stripe's
		// vertices array and the size of a single vertex in it.

		glBindVertexArray(letterDVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_LINE_LOOP, 0, sizeof(verticesOfLetterD) / (5 * sizeof(float)));
		// We draw RESOLUTION_OF_CIRCLE / 2 + 1 vertices, which is the quotient (the result of the division) of the size of
		// letter "D"'s vertices array and the number of data in a vertex.

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_03(GLFWwindow* window, int width, int height)
{
	window_width_for_03_03 = width;
	window_height_for_03_03 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_03(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
