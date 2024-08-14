#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../shader_s.h"
#include "../stb_image.h"

int draw_camera_keyboard_dt();
void framebuffer_size_callback_for_camera_keyboard_dt(GLFWwindow* window, int width, int height);
void processInput_for_camera_keyboard_dt(GLFWwindow* window);
