#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../shader_program.h"
#include "../texture.h"
#include "../camera.h"

int draw_light_casters_spot_smooth();
void framebuffer_size_callback_for_light_casters_spot_smooth(GLFWwindow* window, int width, int height);
void cursor_pos_callback_for_light_casters_spot_smooth(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_for_light_casters_spot_smooth(GLFWwindow* window, double xoffset, double yoffset);
void processInput_for_light_casters_spot_smooth(GLFWwindow* window);
