#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int window_width = 800;
const int window_height = 600;

// Vertex shader, the first stage of the graphics pipeline. Shaders are written in the GLSL language.
const char *vertexShaderSource = "#version 330 core\n" 
"layout (location = 0) in vec3 aPos;\n\n" 
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize the GLFW library.
	if (!glfwInit())
	{
		std::cout << "GLFW library failed to initialize!";

		return 1;
	}
	// Specify the OpenGL version and profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window and make the context of created window the main context on the current thread.
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "StudyingProject", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!";
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!";
		glfwTerminate();

		return 3;
	}

	// Create the vertex shader object.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the vertex shader's source code to the vertex shader object.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Dynamically compile the vertex shader at run-time.
	glCompileShader(vertexShader);

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

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Create memory on the GPU where vertex data will be stored. 
	// Vertex buffer object will be used to handle said data.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// First part: Process the user's input.
		processInput(window);

		// Second part: Rendering commands.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback function.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input processing function.
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
