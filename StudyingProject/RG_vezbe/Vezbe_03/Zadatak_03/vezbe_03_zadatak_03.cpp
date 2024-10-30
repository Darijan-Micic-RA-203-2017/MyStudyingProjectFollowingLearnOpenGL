#include "vezbe_03_zadatak_03.h"

int window_width = 500;
int window_height = 500;

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

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, 
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
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
		 0.7f,  0.7f, 0.5f, 0.0f, 0.5f, 
		 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 
		 0.7f, -0.7f, 0.5f, 0.2f, 1.0f, 
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 
		-0.7f, -0.7f, 0.0f, 0.5f, 0.5f, 
		-1.0f,  1.0f, 0.5f, 0.1f, 0.5f, 
		-0.7f,  0.7f, 0.5f, 0.0f, 0.5f, 
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
		 0.7f,  0.7f, 0.5f, 0.0f, 0.5f
	};
	int strideOfVertexOfBoundingStripe = 5 * sizeof(float);

	unsigned int boundingStripeVAO;
	glGenVertexArrays(1, &boundingStripeVAO);
	unsigned int boundingStripeVBO;
	glGenBuffers(1, &boundingStripeVBO);

	glBindVertexArray(boundingStripeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, boundingStripeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfBoundingStripe), verticesOfBoundingStripe, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, strideOfVertexOfBoundingStripe, (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, strideOfVertexOfBoundingStripe, (void*) (2 * sizeof(float)));
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
		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(verticesOfBoundingStripe) / strideOfVertexOfBoundingStripe);
		// We draw 10 vertices, which is the quotient (the result of the division) of the size of bounding stripe's
		// vertices array and the size of a single vertex in it.

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_03(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_03(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
