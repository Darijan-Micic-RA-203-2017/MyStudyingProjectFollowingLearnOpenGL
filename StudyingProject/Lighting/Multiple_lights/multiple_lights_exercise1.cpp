#include "multiple_lights_exercise1.h"

const int window_width = 800;
const int window_height = 600;

// All settings are kept in an instance of the camera class.
Camera camera_for_3_6_2(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouseEntry_for_3_6_2 = true;
float previousCursorPosX_for_3_6_2 = (float) window_width / 2.0f;
float previousCursorPosY_for_3_6_2 = (float) window_height / 2.0f;

// The time difference between the end of renderings of the current frame and the previous frame.
// We multiply all velocities with delta time value. The result is that when we have a large deltaTime in a frame,
// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to
// balance it all out. When using this approach it does not matter if you have a very fast or slow PC, the velocity
// of the camera will be balanced out accordingly so each user will have the same experience.
float deltaTime_for_3_6_2 = 0.0f;
// The time it took to render the previous frame.
float previousFrameTime_for_3_6_2 = 0.0f;

int draw_multiple_lights_exercise1()
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
		"Lighting - Multiple lights, exercise 1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_multiple_lights_exercise1);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_multiple_lights_exercise1);
	glfwSetScrollCallback(window, scroll_callback_for_multiple_lights_exercise1);

	// Tell GLFW library to capture and hide our mouse cursor. Capturing the mouse cursor means fixating it to the
	// center of application's window and only letting it move if application loses focus or quits.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize the GLAD library.
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD library failed to initialize!" << std::endl;
		glfwTerminate();

		return 3;
	}

	// Configure global OpenGL state.
	// Enable depth testing. Without the use of z-buffer, some sides of the cube will be drawn over other sides
	// of the cube. This happens because when OpenGL draws our cube triangle by triangle, fragment by fragment,
	// it will overwrite any pixel color that may have already been drawn there before. Since OpenGL gives no
	// guarantee on the order of triangles rendered (within the same draw call), some triangles are drawn on
	// top of each other even though one should clearly be in front of the other.
	// Luckily, OpenGL stores depth information in a buffer called the z-buffer that allows OpenGL to decide
	// when to or not to draw over a pixel. Using the z-buffer we can configure OpenGL to do depth testing.
	glEnable(GL_DEPTH_TEST);

	// Compile our shaders and link our shader program using helper class.
	ShaderProgram ourShaderProgram("Lighting_maps/vertex_shader_for_3_4_1.glsl", 
		"Multiple_lights/fragment_shader_for_3_6_1.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
	}
	// Compile our light source shaders and link our light source shader program using helper class.
	ShaderProgram ourLightSourceShaderProgram("Colors/light_source_vertex_shader_for_3_1_1.glsl", 
		"Colors/light_source_fragment_shader_for_3_1_1.glsl");
	if (ourLightSourceShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourLightSourceShaderProgram.errorCode;
	}

	// Vertices in normalized device coordinates system (from -1.0f to 1.0f).
	// We will turn our 2D plane into a 3D cube. In order to render a cube, we need 36 vertices
	// (6 sides * 2 triangles per side * 3 vertices for each triangle).
	// First three values represent position of vertex, middle three values represent vector normal to vertex's
	// surface, while last two values represent texture coordinates (from 0.0f to 1.0f).
	// Vertex's surface is one of 6 planes forming a 3D cube.
	float vertices[] = {
		// Since a vertex by itself doesn't have a surface (vertex is simply a single point in space), we need to
		// use its surrounding vertices to figure out the surface of the vertex.
		// We can use a neat trick to calculate the normal vectors for all the 3D cube's vertices by using the
		// cross product, but since 3D cube is a simple shape we can simply manually add them to the vertex data.
		// position          // normal vector     // texture coordinates
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // back side
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // front side
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // left side
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // right side
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // bottom side
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // top side
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
	};
	// World space positions of our ten cubes.
	glm::vec3 positionsOfCubes[] = {
		glm::vec3(0.0f,  0.0f,  0.0f), 
		glm::vec3(2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f), 
		glm::vec3(-3.8f, -2.0f, -12.3f), 
		glm::vec3(2.4f, -0.4f, -3.5f), 
		glm::vec3(-1.7f,  3.0f, -7.5f), 
		glm::vec3(1.3f, -2.0f, -2.5f), 
		glm::vec3(1.5f,  2.0f, -2.5f), 
		glm::vec3(1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// World space position of our four point light sources.
	glm::vec3 positionsOfPointLightSources[] = {
		glm::vec3(0.7f,  0.2f,  2.0f), 
		glm::vec3(2.3f, -3.3f, -4.0f), 
		glm::vec3(-4.0f,  2.0f, -12.0f), 
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// Colors of our four point light sources.
	glm::vec3 colorsOfPointLightSources[] = {
		glm::vec3(1.0f, 0.6f, 0.0f), 
		glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3(1.0f, 1.0, 0.0), 
		glm::vec3(0.2f, 0.2f, 1.0f)
	};

	// Create memory on the GPU where vertex data and index data will be stored.
	// Said data will be handled by VAO and vertex/element buffer objects inside that VAO.
	// Core OpenGL REQUIRES the use of VAOs!
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind (assign) the newly created VAO to OpenGL's context.
	glBindVertexArray(VAO);

	// Bind (assign) the newly created VBO to OpenGL's context.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy user-defined data into the currently bound buffer.
	// Vertex data is now stored on the graphics card's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	// Position attribute.
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);
	// Normal vector attribute.
	glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	// Enable vertex normal vector attribute.
	glEnableVertexAttribArray(1u);
	// Texture coordinate attribute.
	glVertexAttribPointer(2u, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	// Enable vertex texture coordinate attribute.
	glEnableVertexAttribArray(2u);

	// Create memory on the GPU where vertex data of light source will be stored.
	unsigned int lightSourceVAO;
	glGenVertexArrays(1, &lightSourceVAO);

	// Bind (assign) the newly created VAO to OpenGL's context.
	glBindVertexArray(lightSourceVAO);

	// Bind (assign) the previously created VBO to OpenGL's context. We use the same VBO, because the light source
	// object will use the same vertices as the object in scene (light source is also a 3D cube).
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Tell OpenGL how it should interpret vertex data, per vertex attribute.
	// Position attribute.
	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	// Enable vertex position attribute.
	glEnableVertexAttribArray(0u);

	// Unbind VBO and VAO for safety reasons. This is not neccessary.
	// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls, so
	// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindVertexArray(0u);

	// Generate diffuse map texture, set its wrapping and filtering parameters, load image-to-become-texture from
	// file system and generate all the required mipmaps using helper class.
	Texture diffuseMap("resources/steel-bordered_wooden_container.png");
	if (diffuseMap.errorCode)
	{
		glfwTerminate();

		return diffuseMap.errorCode;
	}
	// Generate specular map texture, set its wrapping and filtering parameters, load image-to-become-texture from
	// file system and generate all the required mipmaps using helper class.
	Texture specularMap("resources/steel_border_specular.png");
	if (specularMap.errorCode)
	{
		glfwTerminate();

		return specularMap.errorCode;
	}

	// Unbind texture for safety reasons. This is not neccessary.
	glBindTexture(GL_TEXTURE_2D, 0u);

	// Draw in wireframe mode. Default polygon rasterization mode is GL_FILL for both sides.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Activate the shader program.
	// Every shader and rendering call from now on will use this shader program object.
	ourShaderProgram.useProgram();
	// Tell OpenGL to which texture unit each shader sampler belongs to, by setting each sampler.
	ourShaderProgram.setIntegerUniform("material.diffuseMap", 0);
	ourShaderProgram.setIntegerUniform("material.specularMap", 1);

	// Rendering loop.
	while (!glfwWindowShouldClose(window))
	{
		// Nullth part: Calculate the new delta time and assign the current frame time to the previous frame time.
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime_for_3_6_2 = currentFrameTime - previousFrameTime_for_3_6_2;
		previousFrameTime_for_3_6_2 = currentFrameTime;

		// First part: Process the user's input.
		processInput_for_multiple_lights_exercise1(window);

		// Second part: Rendering commands.
		glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
		// Since we're now using a depth buffer, we also want to clear it before each rendering iteration.
		// Otherwise, the depth information of the previous frame would remain in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Move light source around the scene over time.
		/*
		float time = static_cast<float>(glfwGetTime());
		positionOfLightSource_for_3_4_2.x = glm::sin(time) * 2.0f + 1.0f;
		positionOfLightSource_for_3_4_2.y = glm::sin(time / 2.0f);
		*/

		// Activate the shader program.
		// Every shader and rendering call from now on will use this shader program object.
		ourShaderProgram.useProgram();

		// Activate texture unit (one of 16). After activating a texture unit, a subsequent "glBindTexture"
		// call will bind that texture to the currently active texture unit. Texture unit "GL_TEXTURE0" is
		// always active by default, so it isn't necessary to manually activate any texture unit if only one
		// texture is used (like in examples previous to "Textures, combined").
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap.id);

		// The projection matrix transforms view space coordinates to clip space coordinates.
		// We will use the perspective projection with varying field of view (FOV) that user sets by scrolling,
		// 0.1f near plane and 100.0f far plane. Ratio of window's width and height is called the aspect ratio.
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera_for_3_6_2.fov), 
			(float) window_width / (float) window_height, 0.1f, 100.0f);
		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by moving the camera using the keyboard.
		glm::mat4 viewMatrix = camera_for_3_6_2.getCalculatedViewMatrix();
		// Set the view matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// Set position of viewer to field "cameraPosition" of global object "camera".
		ourShaderProgram.setFloatVec3Uniform("positionOfViewer", camera_for_3_6_2.cameraPosition);

		// Set all uniforms for all six light sources we have in our scene. We have to set them manually and
		// index the proper "PointLightSource" structure in the array to set each uniform variable. This can be
		// done cleaner by defining each type of light source as a class and setting their values there.
		// Preferably, we should use a more efficient uniform approach called UNIFORM BUFFER OBJECTS. We'll discuss
		// them in the "Advanced GLSL" tutorial.

		// DESERT SCENE:
		// Directional light source.
		ourShaderProgram.setFloatVec3Uniform("directionalLightSource.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		ourShaderProgram.setFloatVec3Uniform("directionalLightSource.ambientColor", glm::vec3(0.3f, 0.24f, 0.14f));
		ourShaderProgram.setFloatVec3Uniform("directionalLightSource.diffuseColor", glm::vec3(0.7f, 0.42f, 0.26f));
		ourShaderProgram.setFloatVec3Uniform("directionalLightSource.specularColor", glm::vec3(0.5f, 0.5f, 0.5f));
		// Point light source #1.
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[0].position", positionsOfPointLightSources[0]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[0].ambientColor", 
			0.1f * colorsOfPointLightSources[0]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[0].diffuseColor", colorsOfPointLightSources[0]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[0].specularColor", colorsOfPointLightSources[0]);
		ourShaderProgram.setFloatUniform("pointLightSources[0].constantParameterOfAttenuation", 1.0f);
		ourShaderProgram.setFloatUniform("pointLightSources[0].linearParameterOfAttenuation", 0.09f);
		ourShaderProgram.setFloatUniform("pointLightSources[0].quadraticParameterOfAttenuation", 0.032f);
		// Point light source #2.
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[1].position", positionsOfPointLightSources[1]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[1].ambientColor", 
			0.1f * colorsOfPointLightSources[1]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[1].diffuseColor", colorsOfPointLightSources[1]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[1].specularColor", colorsOfPointLightSources[1]);
		ourShaderProgram.setFloatUniform("pointLightSources[1].constantParameterOfAttenuation", 1.0f);
		ourShaderProgram.setFloatUniform("pointLightSources[1].linearParameterOfAttenuation", 0.09f);
		ourShaderProgram.setFloatUniform("pointLightSources[1].quadraticParameterOfAttenuation", 0.032f);
		// Point light source #3.
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[2].position", positionsOfPointLightSources[2]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[2].ambientColor", 
			0.1f * colorsOfPointLightSources[2]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[2].diffuseColor", colorsOfPointLightSources[2]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[2].specularColor", colorsOfPointLightSources[2]);
		ourShaderProgram.setFloatUniform("pointLightSources[2].constantParameterOfAttenuation", 1.0f);
		ourShaderProgram.setFloatUniform("pointLightSources[2].linearParameterOfAttenuation", 0.09f);
		ourShaderProgram.setFloatUniform("pointLightSources[2].quadraticParameterOfAttenuation", 0.032f);
		// Point light source #4.
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[3].position", positionsOfPointLightSources[3]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[3].ambientColor", 
			0.1f * colorsOfPointLightSources[3]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[3].diffuseColor", colorsOfPointLightSources[3]);
		ourShaderProgram.setFloatVec3Uniform("pointLightSources[3].specularColor", colorsOfPointLightSources[3]);
		ourShaderProgram.setFloatUniform("pointLightSources[3].constantParameterOfAttenuation", 1.0f);
		ourShaderProgram.setFloatUniform("pointLightSources[3].linearParameterOfAttenuation", 0.09f);
		ourShaderProgram.setFloatUniform("pointLightSources[3].quadraticParameterOfAttenuation", 0.032f);
		// Spotlight.
		ourShaderProgram.setFloatVec3Uniform("spotlight.direction", camera_for_3_6_2.cameraFront);
		ourShaderProgram.setFloatVec3Uniform("spotlight.position", camera_for_3_6_2.cameraPosition);
		ourShaderProgram.setFloatUniform("spotlight.cosOfInnerCutoffAngle", glm::cos(glm::radians(12.5f)));
		ourShaderProgram.setFloatUniform("spotlight.cosOfOuterCutoffAngle", glm::cos(glm::radians(13.0f)));
		ourShaderProgram.setFloatVec3Uniform("spotlight.ambientColor", glm::vec3(0.0f, 0.0f, 0.0f));
		ourShaderProgram.setFloatVec3Uniform("spotlight.diffuseColor", glm::vec3(0.8f, 0.8f, 0.0f));
		ourShaderProgram.setFloatVec3Uniform("spotlight.specularColor", glm::vec3(0.8f, 0.8f, 0.0f));
		ourShaderProgram.setFloatUniform("spotlight.constantParameterOfAttenuation", 1.0f);
		ourShaderProgram.setFloatUniform("spotlight.linearParameterOfAttenuation", 0.09f);
		ourShaderProgram.setFloatUniform("spotlight.quadraticParameterOfAttenuation", 0.032f);

		// Set shininess of highlight to 32. This impacts the scattering and radius of specular highlight.
		float shininessOfHighlight = 32.0f;
		ourShaderProgram.setFloatUniform("material.shininessOfHighlight", shininessOfHighlight);

		// Render 3D cube.
		glBindVertexArray(VAO);
		// We draw ten cubes.
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		for (unsigned int i = 0u; i < 10u; i++)
		{
			// The model matrix transforms local space coordinates to world space coordinates.
			// We will transform every cube by rotating it once around the (1.0f, 0.3f, 0.5f) axis and translating
			// it to its corresponding specified position.
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(positionsOfCubes[i]));
			// GLM's "rotate" function requires the provided angle to be specified in radians, so we convert
			// the angle's value from degrees.
			// The axis we are rotating around should be a unit vector, so make sure to normalize the vector
			// representing the axis if we're not rotating around x, y or z-axis.
			float angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));
			// Set the model matrix. This matrix changes each frame.
			ourShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

			// The normal matrix is a model matrix specifically tailored for normal vectors. Normal matrix is
			// defined as the transpose of the inverse of the upper-left 3x3 part of the model matrix.
			// Non-uniform scaling would transform vertex in such a way that the normal vector would no longer be
			// perpendicular to the vertex's surface. This means that the lighting of surface would be distorted.
			// We mitigate non-uniform scaling by multiplying normal vector with normal matrix.
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
			// Set the normal matrix. This matrix changes each frame.
			ourShaderProgram.setFloatMat3Uniform("normalMatrix", normalMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Activate the light source shader program.
		// Every shader and rendering call from now on will use this shader program object.
		ourLightSourceShaderProgram.useProgram();

		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourLightSourceShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);
		// Set the view matrix. This matrix changes each frame.
		ourLightSourceShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// Render light source, represented by a 3D cube.
		glBindVertexArray(lightSourceVAO);
		// We draw four cubes, representing point light sources.
		for (unsigned int i = 0u; i < 4u; i++)
		{
			// The model matrix transforms local space coordinates to world space coordinates.
			// We will transform 3D cube that is object of our scene, thus copying it to become a representation
			// of our point light source. We will scale it to 1/5 of its initial size and finally translate it to
			// specified position of point light source.
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, positionsOfPointLightSources[i]);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
			// Set the model matrix. This matrix changes each frame.
			ourLightSourceShaderProgram.setFloatMat4Uniform("modelMatrix", modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// On next drawing, reset first mouse entry indicator.
	firstMouseEntry_for_3_6_2 = true;

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback functions.

// Function that will be called every time the application's window changes size.
void framebuffer_size_callback_for_multiple_lights_exercise1(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that will be called every time the user moves the mouse while the application has focus.
void cursor_pos_callback_for_multiple_lights_exercise1(GLFWwindow* window, double xpos, double ypos)
{
	// Calculate "camera's front" vector that acts as insurance that however we move, camera keeps looking
	// straight ahead. Math's explained below.
	// In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);

	// 0. step: if we received mouse input for the first time, we set the previous cursor position to the position
	// where the user entered the application window and calculate offsets based on it. Wihout this added step,
	// camera would suddenly jump to point of mouse entry, which is usually far away from window's center.
	float xPos = static_cast<float>(xpos);
	float yPos = static_cast<float>(ypos);
	if (firstMouseEntry_for_3_6_2)
	{
		previousCursorPosX_for_3_6_2 = xPos;
		previousCursorPosY_for_3_6_2 = yPos;
		firstMouseEntry_for_3_6_2 = false;
	}

	// 1. step: calculate the mouse's offset since last frame.
	float xOffset = xPos - previousCursorPosX_for_3_6_2;
	// Order of subtraction is reversed, because y-coordinates range from bottom to top.
	float yOffset = previousCursorPosY_for_3_6_2 - yPos;
	previousCursorPosX_for_3_6_2 = xPos;
	previousCursorPosY_for_3_6_2 = yPos;

	// 2. step onward: done in Camera class.
	camera_for_3_6_2.processMouseMovement(xOffset, yOffset);
}

// Function that will be called every time the user scrolls the mouse's middle button.
void scroll_callback_for_multiple_lights_exercise1(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_for_3_6_2.processMouseScroll(static_cast<float>(yoffset));
}

// Input processing function.
void processInput_for_multiple_lights_exercise1(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_for_3_6_2.processKeyboardInput("W", deltaTime_for_3_6_2);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_for_3_6_2.processKeyboardInput("S", deltaTime_for_3_6_2);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_for_3_6_2.processKeyboardInput("A", deltaTime_for_3_6_2);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_for_3_6_2.processKeyboardInput("D", deltaTime_for_3_6_2);
	}
}
