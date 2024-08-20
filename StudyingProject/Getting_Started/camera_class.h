#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader_program.h"
#include "stb_image.h"
#include "camera.h"

int draw_camera_class();
void framebuffer_size_callback_for_camera_class(GLFWwindow* window, int width, int height);
void cursor_pos_callback_for_camera_class(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_for_camera_class(GLFWwindow* window, double xoffset, double yoffset);
void processInput_for_camera_class(GLFWwindow* window);
