// Number of discrete points the circle consists of. The circle is a set of connected points bounding the disc (plane area).
// Judging from my own observations, the circle seems round to the eye when its resolution is greater than or equal to 24.
#define RESOLUTION_OF_CIRCLE 30u

#include "vezbe_03_zadatak_08.h"

int window_width_for_03_08 = 800;
int window_height_for_03_08 = 600;

float widthShrinkingFactor_for_03_08 = 1.0f;
float heightShrinkingFactor_for_03_08 = 1.0f;
float color_for_03_08[3u] = { 0.0f, 0.0f, 1.0f };

/* Zadatak 8
Napisati program koji na tamno sivoj pozadini svake 3 sekunde nacrta elipsu (preko prethodnih elipsi). Prva elipsa je duža
po x-osi, druga je duža po y-osi, treća je duža po x-osi, četvrta po y-osi, itd. Svaka naredna elipsa je manja od prethodne
tako da ne izlazi iz nje. Dimenzije i boje elipsi su proizvoljne. Taster ESC zatvara program.
*/
int draw_vezbe_03_zadatak_08()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width_for_03_08, window_height_for_03_08, 
		"Vežbe 3 - zadatak 8", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_vezbe_03_zadatak_08);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	ShaderProgram shaderProgram("Vezbe_03/Zadatak_08/vertex_shader_for_03_08.glsl", 
		"Vezbe_03/Zadatak_08/fragment_shader_for_03_08.glsl");
	if (shaderProgram.errorCode)
	{
		glfwTerminate();

		return shaderProgram.errorCode;
	}

	// The circle has RESOLUTION_OF_CIRCLE vertices, but I have to add 2 more for its center and another drawing of the
	// vertex at the angle of 0.0f degrees.
	float vertices[2u * (RESOLUTION_OF_CIRCLE + 2u)];
	// Set the radius of the circle.
	float r = 0.9f;
	vertices[0u] = 0.0f; vertices[1u] = 0.0f;
	for (unsigned int i = 0u; i <= RESOLUTION_OF_CIRCLE; i++)
	{
		vertices[2u * (1u + i)] = r * cos((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE)));
		vertices[2u * (1u + i) + 1u] = r * sin((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE)));
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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glBindVertexArray(VAO);

	// Reset the GLFW time to 0 before the rendering loop.
	glfwSetTime(0.0);
	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_08(window);

		// I decided to clear the color buffer after all, because it prevents the ellipses from having different centers
		// and thus end up being drawn all over the screen space. This would happen in the previous solution of task 8
		// of practice 3 on every change of the screen space's dimensions or position.
		glClear(GL_COLOR_BUFFER_BIT);

		unsigned int time = static_cast<unsigned int>(glfwGetTime());
		// The height of each odd ellipse and the width of each even ellipse has to be shrinked. I chose to use the loop
		// so that I could use its iterator as the shrinking measure of new, smaller ellipses.
		for (unsigned int i = 0u; i <= time / 3u + 1u; i++)
		{
			if (i % 2u == 0u)
			{
				widthShrinkingFactor_for_03_08 = 1.0f / static_cast<float>(i + 1u);
				color_for_03_08[1u] = 0.0f; color_for_03_08[2u] = 1.0f;
			}
			else
			{
				heightShrinkingFactor_for_03_08 = 1.0f / static_cast<float>(i + 1u);
				color_for_03_08[1u] = 1.0f; color_for_03_08[2u] = 0.0f;
			}
			// Update ellipse width shrinking uniform.
			shaderProgram.setFloatUniform("widthShrinkingFactor", widthShrinkingFactor_for_03_08);
			// Update ellipse height shrinking uniform.
			shaderProgram.setFloatUniform("heightShrinkingFactor", heightShrinkingFactor_for_03_08);
			// Update ellipse color uniform.
			shaderProgram.setFloatVec3Uniform("color", color_for_03_08[0u], color_for_03_08[1u], color_for_03_08[2u]);

			// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
			glDrawArrays(GL_TRIANGLE_FAN, 0, RESOLUTION_OF_CIRCLE + 2);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	widthShrinkingFactor_for_03_08 = 1.0f;
	heightShrinkingFactor_for_03_08 = 1.0f;
	color_for_03_08[1] = 0.0f; color_for_03_08[2] = 1.0f;

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_for_vezbe_03_zadatak_08(GLFWwindow* window, int width, int height)
{
	window_width_for_03_08 = width;
	window_height_for_03_08 = height;

	glViewport(0, 0, width, height);
}

void processInput_for_vezbe_03_zadatak_08(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
