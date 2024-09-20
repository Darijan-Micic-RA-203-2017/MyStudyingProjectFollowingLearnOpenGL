#include "light_casters_spot_smooth.h"

const int window_width = 800;
const int window_height = 600;

// All settings are kept in an instance of the camera class.
Camera camera_for_3_5_4(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouseEntry_for_3_5_4 = true;
float previousCursorPosX_for_3_5_4 = (float) window_width / 2.0f;
float previousCursorPosY_for_3_5_4 = (float) window_height / 2.0f;

// The time difference between the end of renderings of the current frame and the previous frame.
// We multiply all velocities with delta time value. The result is that when we have a large deltaTime in a frame,
// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to
// balance it all out. When using this approach it does not matter if you have a very fast or slow PC, the velocity
// of the camera will be balanced out accordingly so each user will have the same experience.
float deltaTime_for_3_5_4 = 0.0f;
// The time it took to render the previous frame.
float previousFrameTime_for_3_5_4 = 0.0f;

int draw_light_casters_spot_smooth()
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
		"Lighting - Light casters, spot with smooth edges", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window was not created!" << std::endl;
		glfwTerminate();

		return 2;
	}
	glfwMakeContextCurrent(window);

	// Register the callback functions after the window is created and before the render loop is started.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_for_light_casters_spot_smooth);
	glfwSetCursorPosCallback(window, cursor_pos_callback_for_light_casters_spot_smooth);
	glfwSetScrollCallback(window, scroll_callback_for_light_casters_spot_smooth);

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
		"Light_casters/fragment_shader_for_3_5_4.glsl");
	if (ourShaderProgram.errorCode)
	{
		glfwTerminate();

		return ourShaderProgram.errorCode;
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
		deltaTime_for_3_5_4 = currentFrameTime - previousFrameTime_for_3_5_4;
		previousFrameTime_for_3_5_4 = currentFrameTime;

		// First part: Process the user's input.
		processInput_for_light_casters_spot_smooth(window);

		// Second part: Rendering commands.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Since we're now using a depth buffer, we also want to clear it before each rendering iteration.
		// Otherwise, the depth information of the previous frame would remain in the buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Move light source around the scene over time.
		/*
		float time = static_cast<float>(glfwGetTime());
		positionOfLightSource_for_3_4_2.x = glm::sin(time) * 2.0f + 1.0f;
		positionOfLightSource_for_3_4_2.y = glm::sin(time / 2.0f);
		*/

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
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera_for_3_5_4.fov), 
			(float) window_width / (float) window_height, 0.1f, 100.0f);
		// Set the projection matrix. Because we are implementing zooming, this matrix now changes each frame.
		ourShaderProgram.setFloatMat4Uniform("projectionMatrix", projectionMatrix);

		// The view matrix transforms world space coordinates to view space coordinates.
		// We will transform our world (scene) by moving the camera using the keyboard.
		glm::mat4 viewMatrix = camera_for_3_5_4.getCalculatedViewMatrix();
		// Set the view matrix. This matrix changes each frame.
		ourShaderProgram.setFloatMat4Uniform("viewMatrix", viewMatrix);

		// Set position of viewer to field "cameraPosition" of global object "camera".
		ourShaderProgram.setFloatVec3Uniform("positionOfViewer", camera_for_3_5_4.cameraPosition);

		// Spotlight source is a light source with a given position in world space that illuminates light rays
		// ONLY IN A SPECIFIC DIRECTION, with its light rays fading out over distance. That means that only the
		// objects within the inner cone of the spotlight's direction are fully lit. Objects between the inner and
		// the outer cone of the spotlight's direction are partially lit. Objects outside of the outer cone of the
		// spotlight's direction stay dark. A realistic spotlight in OpenGL is represented by:
		// - world-space position;
		// - direction;
		// - inner cutoff angle, specifying the radius of the spotlight;
		// - outer cutoff angle, allowing a gradual reducing of light's intensity through the space between the
		// spotlight's cones.
		// If the fragment is between the inner and the outer cone, its intensity should be in (0.0f, 1.0f) range.
		// If the fragment is inside of the inner cone, its intensity should be CLAMPED TO 1.0f.
		// If the fragment is outside of the outer cone, its intensity should be CLAMPED TO 0.0f.
		// Equation of light's intensity "I" that represents edge clamping factor:
		// I = (cos(theta) - cos(y)) / epsilon = (cos(theta) - cos(y)) / (cos(phi) - cos(y))
		// theta - angle between the "light's direction" and the spotlight's direction
		// phi - inner cutoff angle
		// y - outer cutoff angle

		// Set direction of light to field "cameraFront" of global object "camera".
		ourShaderProgram.setFloatVec3Uniform("lightSource.direction", camera_for_3_5_4.cameraFront);
		// Set position of light source to field "cameraPosition" of global object "camera".
		ourShaderProgram.setFloatVec3Uniform("lightSource.position", camera_for_3_5_4.cameraPosition);
		// Set cosine of inner cutoff angle to 12.5 degrees converted to radians. Inner cutoff angle is the angle
		// between the "light's direction" and the inner cone's vector (equal to its radius).
		// Result of dot product between two vectors is cosine of angle between them. In our case, those two
		// vectors will be the "light's direction" (result of subtracting fragment's position from light source's
		// position) and spotlight direction (camera's front vector). Calculating the inverse cosine is an
		// expensive operation in shaders, so that's why we're sending a cosine of cutoff angle instead of cutoff
		// angle itself.
		ourShaderProgram.setFloatUniform("lightSource.cosOfInnerCutoffAngle", glm::cos(glm::radians(12.5f)));
		// Set cosine of outer cutoff angle to 17.5 degrees converted to radians. Outer cutoff angle is the angle
		// between the "light's direction" and the outer cone's vector (equal to its radius). -||-
		ourShaderProgram.setFloatUniform("lightSource.cosOfOuterCutoffAngle", glm::cos(glm::radians(17.5f)));

		// Change color of light over time.
		glm::vec3 colorOfLight = glm::vec3(1.0f);
		/*
		float time = static_cast<float>(glfwGetTime());
		colorOfLight.x = glm::sin(time * 2.0f);
		colorOfLight.y = glm::sin(time * 0.7f);
		colorOfLight.z = glm::sin(time * 1.3f);
		*/
		// Set ambient component color of light source to (0.1f, 0.1f, 0.1f).
		glm::vec3 ambientColorOfLight = glm::vec3(0.1f) * colorOfLight;
		ourShaderProgram.setFloatVec3Uniform("lightSource.ambientColor", ambientColorOfLight);
		// Set diffuse component color of light source to (0.8f, 0.8f, 0.8f).
		// We will darken the light emitted from light source a bit. Usually it's white (1.0f, 1.0f, 1.0f).
		glm::vec3 diffuseColorOfLight = glm::vec3(0.8f) * colorOfLight;
		ourShaderProgram.setFloatVec3Uniform("lightSource.diffuseColor", diffuseColorOfLight);
		// Set specular component color of light source to (1.0f, 1.0f, 1.0f).
		glm::vec3 specularColorOfLight = glm::vec3(1.0f);
		ourShaderProgram.setFloatVec3Uniform("lightSource.specularColor", specularColorOfLight);

		// The process of reducing the light's intensity over the distance a light ray travels is called ATTENUATION.
		// Simple linear equation would produce unrealistic results which would look fake. Lights in the real
		// world are generally quite bright when standing close by, but their brightness reduces in a linear
		// fashion only up to a certain distance point. At that distance point, equation denominator's quadratic
		// part becomes greater than its linear part and light intensity starts reducing much quicker. Curve of
		// light intensity's reducing eventually becomes flatter, so reducing happens at a slower pace.
		// Equation of attenuation factor "Fatt" for specified distance "d":
		// Fatt = 1.0f / (Kc + Kl * d + Kq * d^2).
		// Choosing the right values of attenuation parameters depends on multiple things:
		// environment, distance we want the light to cover, type of light etc. In our environment, a distance of
		// 32.0f to 100.0f is usually enough for most lights.
		// We want the light to cover the distance of 50.0f units.
		//           constant parameter linear parameter quadratic parameter
		// d = 50.0f:       1.0f             0.09f              0.032f
		// REFERENCE: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation

		// Set constant parameter of attenuation to 1.0f.
		float constantParameterOfAttenuation = 1.0f;
		ourShaderProgram.setFloatUniform("lightSource.constantParameterOfAttenuation", constantParameterOfAttenuation);
		// Set linear parameter of attenuation to 0.09f.
		float linearParameterOfAttenuation = 0.09f;
		ourShaderProgram.setFloatUniform("lightSource.linearParameterOfAttenuation", linearParameterOfAttenuation);
		// Set quadratic parameter of attenuation to 0.032f.
		float quadraticParameterOfAttenuation = 0.032f;
		ourShaderProgram.setFloatUniform("lightSource.quadraticParameterOfAttenuation", quadraticParameterOfAttenuation);

		// Set shininess of highlight to 32. This impacts the scattering and radius of specular highlight.
		float shininessOfHighlight = 32.0f;
		ourShaderProgram.setFloatUniform("material.shininessOfHighlight", shininessOfHighlight);

		// Render 3D cube.
		glBindVertexArray(VAO);
		// We draw ten cubes.
		for (unsigned int i = 0u; i < 10u; i++)
		{
			// The model matrix transforms local space coordinates to world space coordinates.
			// We will transform every cube by rotating it once around the (1.0f, 0.3f, 0.5f) axis and translating
			// it to its corresponding specified position.
			glm::mat4 modelMatrix = glm::mat4(1.0f);
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

		// Third part: Swap buffers, check for events and call the events if they occured.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// On next drawing, reset first mouse entry indicator.
	firstMouseEntry_for_3_5_4 = true;

	// De-allocate all resources once they're no longer needed.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Terminate the GLFW library, which frees up all allocated resources.
	glfwTerminate();

	return 0;
}

// Callback functions.

// Function that will be called every time the application's window changes size.
void framebuffer_size_callback_for_light_casters_spot_smooth(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that will be called every time the user moves the mouse while the application has focus.
void cursor_pos_callback_for_light_casters_spot_smooth(GLFWwindow* window, double xpos, double ypos)
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
	if (firstMouseEntry_for_3_5_4)
	{
		previousCursorPosX_for_3_5_4 = xPos;
		previousCursorPosY_for_3_5_4 = yPos;
		firstMouseEntry_for_3_5_4 = false;
	}

	// 1. step: calculate the mouse's offset since last frame.
	float xOffset = xPos - previousCursorPosX_for_3_5_4;
	// Order of subtraction is reversed, because y-coordinates range from bottom to top.
	float yOffset = previousCursorPosY_for_3_5_4 - yPos;
	previousCursorPosX_for_3_5_4 = xPos;
	previousCursorPosY_for_3_5_4 = yPos;

	// 2. step onward: done in Camera class.
	camera_for_3_5_4.processMouseMovement(xOffset, yOffset);
}

// Function that will be called every time the user scrolls the mouse's middle button.
void scroll_callback_for_light_casters_spot_smooth(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_for_3_5_4.processMouseScroll(static_cast<float>(yoffset));
}

// Input processing function.
void processInput_for_light_casters_spot_smooth(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_for_3_5_4.processKeyboardInput("W", deltaTime_for_3_5_4);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_for_3_5_4.processKeyboardInput("S", deltaTime_for_3_5_4);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_for_3_5_4.processKeyboardInput("A", deltaTime_for_3_5_4);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_for_3_5_4.processKeyboardInput("D", deltaTime_for_3_5_4);
	}
}
