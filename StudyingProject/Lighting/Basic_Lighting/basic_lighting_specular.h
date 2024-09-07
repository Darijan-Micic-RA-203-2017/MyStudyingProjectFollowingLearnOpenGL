#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../shader_program.h"
#include "../camera.h"

int draw_basic_lighting_specular();
void framebuffer_size_callback_for_basic_lighting_specular(GLFWwindow* window, int width, int height);
void cursor_pos_callback_for_basic_lighting_specular(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_for_basic_lighting_specular(GLFWwindow* window, double xoffset, double yoffset);
void processInput_for_basic_lighting_specular(GLFWwindow* window);
