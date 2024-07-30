#include "hello_triangle_exercise2.h"

const int window_width = 800;
const int window_height = 600;

// Vertex shader, the first stage of the graphics pipeline. Shaders are written in the GLSL language.
const char* vertexShaderSource_for_2_4_4 = "#version 330 core\n\n"
"layout (location = 0) in vec3 aPos;\n\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";
// Fragment shader, the fifth stage of the graphics pipeline. Shaders are written in the GLSL language.
const char* fragmentShaderSource_for_2_4_4 = "#version 330 core\n\n"
"out vec4 FragColor;\n\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int draw_hello_triangle_exercise2()
{
	// Initialize the GLFW library.
	if (!glfwInit())
	{
		std::cout << "GLFW library failed to initialize!" << std::endl;

		return 1;
	}
	// Specify the OpenGL version and profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window and make the context of created window the main context on the current thread.
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, 
		"StudyingProject - Hello Triangle, exercise 2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_hello_triangle_exercise2);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Create the vertex shader object.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the vertex shader's source code to the vertex shader object.
	glShaderSource(vertexShader, 1, &vertexShaderSource_for_2_4_4, NULL);
	// Dynamically compile the vertex shader at run-time.
	glCompileShader(vertexShader);

	// Check whether the compilation of vertex shader succeeded and print out the error if it didn't.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Compilation of vertex shader has failed!\n" << infoLog << std::endl;
		glfwTerminate();

		return 4;
	}

	// Create the fragment shader object.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach the fragment shader's source code to the fragment shader object.
	glShaderSource(fragmentShader, 1, &fragmentShaderSource_for_2_4_4, NULL);
	// Dynamically compile the fragment shader at run-time.
	glCompileShader(fragmentShader);

	// Check whether the compilation of fragment shader succeeded and print out the error if it didn't.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Compilation of fragment shader has failed!\n" << infoLog << std::endl;
		glfwTerminate();

		return 5;
	}

	// Create the shader program object.
	unsigned int shaderProgram = glCreateProgram();
	// Attach the previously compiled shaders to shader program.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link previously compiled shaders into a program.
	// The outputs of each shader are linked to the inputs of next shader.
	glLinkProgram(shaderProgram);

	// Check whether the linking of shader program succeeded and print out the error if it didn't.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Linking of shader program has failed!\n" << infoLog << std::endl;
		glfwTerminate();

		return 6;
	}

	// Delete shader objects after linking, we no longer need them.
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	float vertices_of_first_triangle[] = {
		-0.9f,  -0.5f, 0.0f, // left
		 0.0f,  -0.5f, 0.0f, // right
		-0.45f,  0.5f, 0.0f, // top
	};
	float vertices_of_second_triangle[] = {
		 0.0f,  -0.5f, 0.0f, // left
		 0.9f,  -0.5f, 0.0f, // right
		 0.45f,  0.5f, 0.0f  // top
	};

	// Create memory on the GPU where vertex data and index data will be stored.
	// Said data will be handled by VAO and vertex/element buffer objects inside that VAO.
	// Core OpenGL REQUIRES the use of VAOs!
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);
	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);

	// Bind (assign) the newly created VAO to OpenGL's context. Set up first triangle.
	glBindVertexArray(VAOs[0]);

	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_of_first_triangle), vertices_of_first_triangle, 
		GL_STATIC_DRAW);
	
	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	// Enable vertex attribute.
	glEnableVertexAttribArray(0);

	// Bind (assign) the newly created VAO to OpenGL's context. Set up second triangle.
	glBindVertexArray(VAOs[1]);

	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_of_second_triangle), vertices_of_second_triangle, 
		GL_STATIC_DRAW);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	// Enable vertex attribute.
	glEnableVertexAttribArray(0);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// First part: Process the user's input.
		processInput_for_hello_triangle_exercise2(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader program.
		// Every shader and rendering call from now on will use this shader program object.
		glUseProgram(shaderProgram);
		// Draw first triangle, using data set up in first VAO.
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Draw second triangle, using data set up in second VAO.
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(2, VBOs);
	glDeleteVertexArrays(2, VAOs);
	glDeleteProgram(shaderProgram);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback function.
void framebuffer_size_callback_for_hello_triangle_exercise2(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input processing function.
void processInput_for_hello_triangle_exercise2(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
