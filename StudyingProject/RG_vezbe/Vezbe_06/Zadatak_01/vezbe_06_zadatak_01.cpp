#include "vezbe_06_zadatak_01.h"

int window_width_for_06_01 = 800;
int window_height_for_06_01 = 600;

float mixingFactor_for_06_01 = 1.0f;

float deltaTime_for_06_01 = 0.0f;
float previousFrameTime_for_06_01 = 0.0f;

/* Zadatak 1
-----------------------------------------------------------------------------------------------------------------------
Tekst zadatka 1 sa vežbi 5:
Nacrtati kvadrat i "obući ga" dvema teksturama. Tokom vremena, jedna tekstura treba da se kreće s desna na levo, a
druga od dole ka gore. Strelicama (nagore i nadole) upravlja se stepenom mešanja dve teksture.
-----------------------------------------------------------------------------------------------------------------------
Nadograditi rešenje zadatka 1 sa vežbi 5.
*/
int draw_vezbe_06_zadatak_01()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_06_01, window_height_for_06_01, 
		"Vežbe 6 - zadatak 1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_06_zadatak_01);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_06/Zadatak_01/vertex_shader_for_06_01.glsl", 
		"Vezbe_06/Zadatak_01/fragment_shader_for_06_01.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	float vertices[] = {
		// position     // texture coordinates
		-0.75f, -0.75f, 0.0f, 0.0f, 
		 0.75f, -0.75f, 1.0f, 0.0f, 
		-0.75f,  0.75f, 0.0f, 1.0f, 
		 0.75f,  0.75f, 1.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);
	glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	Texture checkerboardTexture("resources/checkerboard_green_blue_and_yellow.png");
	if (checkerboardTexture.errorCode)
	{
		glfwTerminate();

		return checkerboardTexture.errorCode;
	}
	Texture dragonTexture("resources/dragon_orange.png");
	if (dragonTexture.errorCode)
	{
		glfwTerminate();

		return dragonTexture.errorCode;
	}
	glBindTexture(GL_TEXTURE_2D, 0u);

	shaderProgram.useProgram();

	shaderProgram.setIntegerUniform("texture0", 0);
	shaderProgram.setIntegerUniform("texture1", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_06_01 = currentFrameTime - previousFrameTime_for_06_01;
		previousFrameTime_for_06_01 = currentFrameTime;

		processInput_for_vezbe_06_zadatak_01(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update textures movement uniforms.
		float scaledTime = 0.25f * currentFrameTime;
		shaderProgram.setFloatUniform("movementOfTexture0", scaledTime);
		shaderProgram.setFloatUniform("movementOfTexture1", -scaledTime);
		// Update texture mixing factor uniform.
		shaderProgram.setFloatUniform("mixingFactor", mixingFactor_for_06_01);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, checkerboardTexture.id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dragonTexture.id);

		glBindVertexArray(VAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_06_zadatak_01(GLFWwindow* window, int width, int height)
{
	window_width_for_06_01 = width;
	window_height_for_06_01 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_06_zadatak_01(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float speedOfMixingFactorChange = 0.3f * deltaTime_for_06_01;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixingFactor_for_06_01 += speedOfMixingFactorChange;
		if (mixingFactor_for_06_01 > 1.0f)
		{
			mixingFactor_for_06_01 = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixingFactor_for_06_01 -= speedOfMixingFactorChange;
		if (mixingFactor_for_06_01 < 0.0f)
		{
			mixingFactor_for_06_01 = 0.0f;
		}
	}
}
