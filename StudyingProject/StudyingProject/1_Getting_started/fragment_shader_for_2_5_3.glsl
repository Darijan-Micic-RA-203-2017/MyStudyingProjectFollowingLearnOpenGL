#version 330 core

// in vec4 ourColor;
in vec3 ourPosition;

out vec4 FragColor;

void main()
{
    // FragColor = ourColor;
    FragColor = vec4(ourPosition, 1.0f);
}
