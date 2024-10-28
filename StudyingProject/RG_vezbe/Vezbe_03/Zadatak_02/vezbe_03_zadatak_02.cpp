// Number of discrete points the circle consists of. The circle is a set of connected points bounding the disc (plane area).
// Judging from my own observations, circle seems continuous to the eye when its resolution is greater than or equal to 24.
#define RESOLUTION_OF_CIRCLE 30u

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

	ShaderProgram serbianFlagShaderProgram("Vezbe_03/Zadatak_02/vertex_shader_of_serbian_flag_for_03_02.glsl", 
		"Vezbe_03/Zadatak_02/fragment_shader_of_serbian_flag_for_03_02.glsl");
	if (serbianFlagShaderProgram.errorCode)
	{
		glfwTerminate();

		return serbianFlagShaderProgram.errorCode;
	}
	ShaderProgram japaneseFlagShaderProgram("Vezbe_03/Zadatak_02/vertex_shader_of_japanese_flag_for_03_02.glsl", 
		"Vezbe_03/Zadatak_02/fragment_shader_of_japanese_flag_for_03_02.glsl");
	if (japaneseFlagShaderProgram.errorCode)
	{
		glfwTerminate();

		return japaneseFlagShaderProgram.errorCode;
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

	// We draw the circle using the "GL_TRIANGLE_FAN" primitive. The circle is approximated as a polygon - collection of
	// discrete points at the same distance from the central point. The rasterization connects these discrete points by
	// straight lines. The total number of vertices we need is 2 * circle's resolution + 4. We multiply circle's resolution
	// with the number of needed coordinates (2, x and y) and add 4 vertices for circle's center and angle of zero degrees.
	float verticesOfJapaneseFlag[2u * RESOLUTION_OF_CIRCLE + 4u];
	// Set the radius of the circle.
	float r = 0.5f;
	// Set the first vertex to be the center of the circle.
	verticesOfJapaneseFlag[0u] = 0.0f;
	verticesOfJapaneseFlag[1u] = 0.0f;
	// In order to close the circle, we need to add the (cos(0.0f), sin(0.0f)) vertex twice.
	// That's why we use the less or equal ("<=") comparator in the "for" loop.
	for (unsigned int i = 0u; i <= RESOLUTION_OF_CIRCLE; i++)
	{
		// The polygon approximating the circle will have RESOLUTION_OF_CIRCLE vertices. Each vertex of that polygon is at
		// 360.0f degrees / RESOLUTION_OF_CIRCLE angle from the center of the circle.
		// Computers are working with radian values of angles, so we need to convert the angle from degrees to radians. We
		// do so by multiplying it with pi and dividing it by 180.
		// Therefore, x-coordinate and y-coordinate of the point on circle are calculated by the following equations:
		// Vx = r * cos((pi / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE))).
		// Vy = r * sin((pi / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE))).
		verticesOfJapaneseFlag[2u + 2u * i] = r * cos((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE)));
		verticesOfJapaneseFlag[2u + 2u * i + 1u] = r * sin((3.141592f / 180.0f) * (i * (360.0f / RESOLUTION_OF_CIRCLE)));
	}

	unsigned int serbianFlagVAO, japaneseFlagVAO;
	glGenVertexArrays(1, &serbianFlagVAO);
	glGenVertexArrays(1, &japaneseFlagVAO);
	unsigned int serbianFlagVBO, japaneseFlagVBO;
	glGenBuffers(1, &serbianFlagVBO);
	glGenBuffers(1, &japaneseFlagVBO);
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

	glBindVertexArray(japaneseFlagVAO);

	glBindBuffer(GL_ARRAY_BUFFER, japaneseFlagVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfJapaneseFlag), verticesOfJapaneseFlag, GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0u);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	while (!glfwWindowShouldClose(window))
	{
		processInput_for_vezbe_03_zadatak_02(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		serbianFlagShaderProgram.useProgram();

		// The first two parameters of "glViewport" function are the coordinates of the bottom left corner of the
		// screen space, while the last two parameters are the width and the height of screen space.
		glViewport(0, 0, window_width_for_03_02 / 2, window_height_for_03_02);
		glBindVertexArray(serbianFlagVAO);
		// Parameters: primitive, total number of indices to be read, type of indices, offset from the beginning of the
		// indices array (after which we start reading the previously specified total number of indices to be read).
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*) 0);

		japaneseFlagShaderProgram.useProgram();

		// The first two parameters of "glViewport" function are the coordinates of the bottom left corner of the
		// screen space, while the last two parameters are the width and the height of screen space.
		glViewport(window_width_for_03_02 / 2, 0, window_width_for_03_02 / 2, window_height_for_03_02);
		glBindVertexArray(japaneseFlagVAO);
		// Parameters: primitive, index of first vertex to be drawn, total number of vertices to be drawn.
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(verticesOfJapaneseFlag));

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
