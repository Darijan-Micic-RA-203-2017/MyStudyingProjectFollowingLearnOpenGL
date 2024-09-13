#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../shader_program.h"
#include "../texture.h"
#include "../camera.h"

int draw_lighting_maps_diffuse();
void framebuffer_size_callback_for_lighting_maps_diffuse(GLFWwindow* window, int width, int height);
void cursor_pos_callback_for_lighting_maps_diffuse(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_for_lighting_maps_diffuse(GLFWwindow* window, double xoffset, double yoffset);
void processInput_for_lighting_maps_diffuse(GLFWwindow* window);
